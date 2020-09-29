import os
import re
import subprocess
import sys

import fiona as fiona
import rasterio
from rasterio.windows import Window
import rasterio.features
import rasterio.warp
import rasterio.mask
from lxml import etree
import shutil

def create_roi(product_path, vector_file, window):
    print('Crop window', window)
    file_in = None
    for pth in os.listdir(product_path):
        if os.path.splitext(pth)[1] == '.TIF':
            file_in = os.path.join(product_path, pth)
    print('Crop input TIF:', file_in)
    with rasterio.open(file_in) as src:
        kwargs = src.meta.copy()
        gt = rasterio.windows.transform(window, src.transform)
        kwargs.update({
            'height': window.height,
            'width': window.width,
            'transform': gt })
        file_out = os.path.join(os.path.dirname(vector_file), 'temp.tif')
        with rasterio.open(file_out, 'w', **kwargs) as dst:
            dst.write(src.read(window=window))

        subprocess.call(['gdaltindex', vector_file, file_out])
        os.remove(file_out)
        return gt

def crop_product_images(output_path, product_path, shp_file):
    out_product_path = os.path.join(output_path, os.path.basename(product_path))
    os.makedirs(out_product_path)
    for pth in os.listdir(product_path):
        file_in = os.path.join(product_path, pth)
        if os.path.splitext(pth)[1] == '.TIF':
            with fiona.open(shp_file, "r") as shapefile:
                shapes = [feature["geometry"] for feature in shapefile]
            file_out = os.path.join(out_product_path, pth)
            with rasterio.open(file_in) as src:
                out_image, out_transform = rasterio.mask.mask(src, shapes, crop=True)
                out_meta = src.meta
                out_meta.update({"driver": "GTiff",
                                 "height": out_image.shape[1],
                                 "width": out_image.shape[2],
                                 "transform": out_transform})

                with rasterio.open(file_out, "w", **out_meta) as dest:
                    dest.write(out_image)
        else:
            shutil.copy(file_in, out_product_path)

def main(context_dir, product_type, output_path=None,  offset=10, crop_size=480):
    if output_path is None:
        output_path = os.path.join(os.path.dirname(context_dir),
                               os.path.basename(context_dir) + '_cropped')
        os.makedirs(output_path, exist_ok=True)

    product_dir_pattern = {
        'LANDSAT8': 'LC08_*',
        'LANDSAT8_MUSCATE': 'LANDSAT8*_L1C_*',
        'SENTINEL2': 'S2A*.SAFE'
    }
    window = Window(offset, offset, crop_size, crop_size)
    vector_file = os.path.join(output_path, 'roi.gml')
    product_path = get_matching_dirs(context_dir, product_dir_pattern[product_type])[0]
    gt = create_roi(product_path, vector_file, window)
    ulx = gt[2]
    uly = gt[5]
    #gdal.Info(vector_file)
    for p in os.listdir(context_dir):
        abs_path = os.path.join(context_dir, p)
        if os.path.isdir(abs_path):
            if '_GIP_' in p:
                shutil.copytree(abs_path, os.path.join(output_path, p))
            elif re.match(product_dir_pattern[product_type], p):
                crop_product_images(output_path, abs_path, vector_file)
            elif '_AUX_REFDE2_' in p:
                crop_product_images(output_path, abs_path, vector_file)
            else:
                print("unknown directory", abs_path)
        else:
            if p.endswith('.HDR') and '_AUX_REFDE2_' in p:
                print("Fixing", abs_path, '...')
                root = etree.fromstring(open(abs_path).read().encode())
                xpath = "./Variable_Header/Specific_Product_Header/DEM_Information/Cartographic"
                node_carto = root.findall(xpath, namespaces=root.nsmap)[0]
                node_ulx = node_carto.findall("./Upper_Left_Corner/X", namespaces=root.nsmap)[0]
                node_uly = node_carto.findall("./Upper_Left_Corner/Y", namespaces=root.nsmap)[0]
                node_ulx.text = str(ulx)
                node_uly.text = str(ulx)
                node_lines = node_carto.findall("./Size/Lines", namespaces=root.nsmap)[0]
                node_columns = node_carto.findall("./Size/Columns", namespaces=root.nsmap)[0]
                node_lines.text = str(crop_size)
                node_columns.text = str(crop_size)
                with open(os.path.join(output_path, p), 'w') as fp:
                    ##print(etree.tostring(root, pretty_print=True))
                    fp.write(etree.tostring(root, pretty_print=True).decode())
            else:
                shutil.copy(abs_path, output_path)

    #os.remove(vector_file)
    #os.remove(vector_file_xsd)
    print('Cropped context in:', output_path)

def get_matching_dirs(context_dir, pattern):
    results = []
    for root, dirs, files in os.walk(context_dir):
        for d in dirs:
            if re.match(pattern, d):
                results.append(os.path.join(root,d))

    return results

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage:", sys.argv[0], " <path-to-TV-input-context>  <product_type> [output_dir] [offset](default is 10) [crop_size](default is 480)")
        print("Usage:", sys.argv[0], "/data/TV/Input/L8-V-L2INIT-001-F-N-LANDSAT8-GENERAL LANDSAT8")
        sys.exit(1)
    context_dir = sys.argv[1]
    product_type = sys.argv[2]
    output_dir = None if len(sys.argv) < 4 else sys.argv[3]
    offset = 10 if len(sys.argv) < 5 else int(sys.argv[4])
    crop_size = 480 if len(sys.argv) < 6 else int(sys.argv[5])
    print("using crop offset", offset)
    print("using crop size", crop_size)
    main(context_dir, product_type, output_dir, offset, crop_size)
