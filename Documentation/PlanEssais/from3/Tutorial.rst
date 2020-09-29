.. role:: math(raw)
   :format: html latex
..

Building Simple Applications with OTB
=====================================

[chap:Tutorials]

Well, that’s it, you’ve just downloaded and installed OTB, lured by the
promise that you will be able to do everything with it. That’s true, you
will be able to do everything but - there is always a *but* - some
effort is required.

OTB uses the very powerful systems of generic programing, many classes
are already available, some powerful tools are defined to help you with
recurrent tasks, but it is not an easy world to enter.

These tutorials are designed to help you enter this world and grasp the
logic behind OTB. Each of these tutorials should not take more than 10
minutes (typing included) and each is designed to highlight a specific
point. You may not be concerned by the latest tutorials but it is
strongly advised to go through the first few which cover the basics
you’ll use almost everywhere.

Hello world
-----------

[sec:TutorialHelloWorld]

Let’s start by the typical *Hello world* program. We are going to
compile this C++ program linking to your new OTB.

First, create a new folder to put your new programs (all the examples
from this tutorial) in and go into this folder.

Since all programs using OTB are handled using the CMake system, we need
to create a that will be used by CMake to compile our program. An
example of this file can be found in the directory. The will be very
similar between your projects.

Open the file and write in the few lines:

::

    PROJECT(Tutorials)

    cmake_minimum_required(VERSION 2.6)

    FIND_PACKAGE(OTB)
    IF(OTB_FOUND)
      INCLUDE(${OTB_USE_FILE})
    ELSE(OTB_FOUND)
      MESSAGE(FATAL_ERROR
          "Cannot build OTB project without OTB.  Please set OTB_DIR.")
    ENDIF(OTB_FOUND)

    ADD_EXECUTABLE(HelloWorldOTB HelloWorldOTB.cxx )
    TARGET_LINK_LIBRARIES(HelloWorldOTB OTBCommon OTBIO)

The first line defines the name of your project as it appears in Visual
Studio (it will have no effect under UNIX or Linux). The second line
loads a CMake file with a predefined strategy for finding OTB  [1]_. If
the strategy for finding OTB fails, CMake will prompt you for the
directory where OTB is installed in your system. In that case you will
write this information in the variable. The line loads the file to set
all the configuration information from OTB.

The line defines as its first argument the name of the executable that
will be produced as result of this project. The remaining arguments of
are the names of the source files to be compiled and linked. Finally,
the line specifies which OTB libraries will be linked against this
project.

HelloWorldOTB.tex

Once the file is written, run on the current directory (that is under
Linux/Unix). If OTB is on a non standard place, you will have to tell
CMake where it is. Once your done with CMake (you shouldn’t have to do
it anymore) run .

You finally have your program. When you run it, you will have the *OTB
Hello World !* printed.

Ok, well done! You’ve just compiled and executed your first OTB program.
Actually, using OTB for that is not very useful, and we doubt that you
downloaded OTB only to do that. It’s time to move on to a more advanced
level.

Pipeline basics: read and write
-------------------------------

[sec:TutorialPipeline]

OTB is designed to read images, process them and write them to disk or
view the result. In this tutorial, we are going to see how to read and
write images and the basics of the pipeline system.

First, let’s add the following lines at the end of the file:

::

    ADD_EXECUTABLE(Pipeline Pipeline.cxx )
    TARGET_LINK_LIBRARIES(Pipeline OTBCommon OTBIO)

Now, create a file.

Pipeline.tex

Once this file is written you just have to run . The call is not
required anymore.

Get one image from the directory in the OTB sources. For example get .

Now, run your new program as . You obtain the file which is the same
image as . When you triggered the method, OTB opened the original image
and wrote it back under another name.

Well…that’s nice but a bit complicated for a copy program!

Wait a minute! We didn’t specify the file format anywhere! Let’s try .
And voila! The output image is a jpeg file.

That’s starting to be a bit more interesting: this is not just a program
to copy image files, but also to convert between image formats.

You have just experienced the pipeline structure which executes the
filters only when needed and the automatic image format detection.

Now it’s time to do some processing in between.

Filtering pipeline
------------------

[sec:TutorialFiltering]

We are now going to insert a simple filter to do some processing between
the reader and the writer.

Let’s first add the 2 following lines to the file:

::

    ADD_EXECUTABLE(FilteringPipeline FilteringPipeline.cxx )
    TARGET_LINK_LIBRARIES(FilteringPipeline OTBCommon OTBIO)

Compile with and execute as .

You have the filtered version of your image in the file.

Now, you can practice a bit and try to replace the filter by one of the
150+ filters which inherit from the class. You will definitely find some
useful filters here!

Handling types: scaling output
------------------------------

[sec:TutorialScaling]

If you tried some other filter in the previous example, you may have
noticed that in some cases, it does not make sense to save the output
directly as an integer. This is the case if you tried the . If you tried
to use it directly in the previous example, you will have some warning
about converting to unsigned char from double.

The output of the Canny edge detection is a floating point number. A
simple solution would be to used double as the pixel type.
Unfortunately, most image formats use integer typed and you should
convert the result to an integer image if you still want to visualize
your images with your usual viewer (we will see in a tutorial later how
you can avoid that using the built-in viewer).

To realize this conversion, we will use the .

Add the two lines to the file:

::

    ADD_EXECUTABLE(ScalingPipeline ScalingPipeline.cxx )
    TARGET_LINK_LIBRARIES(ScalingPipeline OTBCommon OTBIO)

As you should be getting used to it by now, compile with and execute as
.

You have the filtered version of your image in the file.

Working with multispectral or color images
------------------------------------------

So far, as you may have noticed, we have been working with grey level
images, i.e. with only one spectral band. If you tried to process a
color image with some of the previous examples you have probably
obtained a deceiving grey result.

Often, satellite images combine several spectral band to help the
identification of materials: this is called multispectral imagery. In
this tutorial, we are going to explore some of the mechanisms used by
OTB to process multispectral images.

Add the following lines in the file:

::

    ADD_EXECUTABLE(Multispectral Multispectral.cxx )
    TARGET_LINK_LIBRARIES(Multispectral OTBCommon OTBIO)

Compile with and execute as .

Parsing command line arguments
------------------------------

[sec:TutorialParsing]

Well, if you play with some other filters in the previous example, you
probably noticed that in many cases, you need to set some parameters to
the filters. Ideally, you want to set some of these parameters from the
command line.

In OTB, there is a mechanism to help you parse the command line
parameters. Let try it!

Add the following lines in the file:

::

    ADD_EXECUTABLE(SmarterFilteringPipeline SmarterFilteringPipeline.cxx )
    TARGET_LINK_LIBRARIES(SmarterFilteringPipeline OTBCommon OTBIO)

Compile with as usual. The execution is a bit different now as we have
an automatic parsing of the command line. First, try to execute as
without any argument.

The usage message (automatically generated) appears:

::

    '--InputImage' option is obligatory !!!

     Usage : ./SmarterFilteringPipeline
          [--help|-h]           :  Help
          [--version|-v]        :  Version
           --InputImage|-in     :  input image file name   (1 parameter)
           --OutputImage|-out   :  output image file name   (1 parameter)
          [--SigmaD|-d]         :  Set the sigmaD parameter of the Harris points of
    interest  algorithm. Default is 1.0.  (1 parameter)
          [--SigmaI|-i]         :  Set the SigmaI parameter of the Harris points of
    interest  algorithm. Default is 1.0.  (1 parameter)
          [--Alpha|-a]          :  Set the alpha parameter of the Harris points of
    interest  algorithm. Default is 1.0.  (1 parameter)

That looks a bit more professional: another user should be able to play
with your program. As this is automatic, that’s a good way not to forget
to document your programs.

So now you have a better idea of the command line options that are
possible. Try for a basic version with the default values.

If you want a result that looks a bit better, you have to adjust the
parameter with for example.

Viewer
------

[sec:TutorialViewer]

So far, we had to save the image and use an external viewer every time
we wanted to see the result of our processing. That is not very
convenient, especially for some *exotic* formats (16 bits, floating
point …). Thankfully, OTB comes with it’s own visualization tool.

This tool is accessible by the class . We will now design a simple,
minimalistic example to illustrate the use for this viewer.

First you need to add the following lines in the file:

::

    ADD_EXECUTABLE(SimpleViewer SimpleViewer.cxx )
    TARGET_LINK_LIBRARIES(SimpleViewer OTBCommon OTBIO OTBGui OTBVisualization)

Notice that you have to link to 2 other OTB libraries: OTBGui and
OTBVisualization.

After compiling you can execute the program with . The result of the
edge detection is displayed. Notice that you can call this simple
program with a big image (let’s say :math:`30000 \times 30000` pixels).
For all multithreaded filters (filters which implement a method), the
image is splitted into piece and only the piece on display is processed.

Going from raw satellite images to useful products
--------------------------------------------------

Quite often, when you buy satellite images, you end up with several
images. In the case of optical satellite, you often have a panchromatic
spectral band with the highest spatial resolution and a multispectral
product of the same area with a lower resolution. The resolution ratio
is likely to be around 4.

To get the best of the image processing algorithms, you want to combine
these data to produce a new image with the highest spatial resolution
and several spectral band. This step is called fusion and you can find
more details about it in [sec:Fusion]. However, the fusion suppose that
your two images represents exactly the same area. There are different
solutions to process your data to reach this situation. Here we are
going to use the metadata available with the images to produce an
orthorectification as detailled in [sec:Ortho].

First you need to add the following lines in the file:

::

    ADD_EXECUTABLE(OrthoFusion  OrthoFusion.cxx)
    TARGET_LINK_LIBRARIES(OrthoFusion OTBProjections OTBCommon OTBIO)

.. [1]
   Similar files are provided in CMake for other commonly used
   libraries, all of them named
