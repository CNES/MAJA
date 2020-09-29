System Overview
===============

[chapter:SystemOverview]

The purpose of this chapter is to provide you with an overview of the
*ORFEO Toolbox* system. We recommend that you read this chapter to gain
an appreciation for the breadth and area of application of OTB. In this
chapter, we will make reference either to *OTB features* or *ITK
features* without distinction. Bear in mind that OTB uses ITK as its
core element, so all the fundamental elements of OTB come from ITK. OTB
extends the functionalities of ITK for the remote sensing image
processing comunity. We benefit from the Open Source development
approach chosen for ITK, which allows us to provide an impressive set of
functionalities with much lesser effort than it would have been the case
in a closed source universe!

System Organization
-------------------

[sec:SystemOrganization]

The ORFEO Toolbox consists of several subsystems. A brief description of
these subsystems follows. Later sections in this chapter—and in some
cases additional chapters—cover these concepts in more detail. (Note: in
the previous chapter two other modules— and were briefly described.)

Essential System Concepts.
    Like any software system, OTB is built around some core design
    concepts. OTB uses those of ITK. Some of the more important concepts
    include generic programming, smart pointers for memory management,
    object factories for adaptable object instantiation, event
    management using the command/observer design paradigm, and
    multithreading support.

Numerics
    OTB, as ITK uses VXL’s VNL numerics libraries. These are easy-to-use
    C++ wrappers around the Netlib Fortran numerical analysis routines
    (http://www.netlib.org).

Data Representation and Access.
    Two principal classes are used to represent data: the and classes.
    In addition, various types of iterators and containers are used in
    ITK to hold and traverse the data. Other important but less popular
    classes are also used to represent data such as histograms.

ITK’s Data Processing Pipeline.
    The data representation classes (known as *data objects*) are
    operated on by *filters* that in turn may be organized into data
    flow *pipelines*. These pipelines maintain state and therefore
    execute only when necessary. They also support multi-threading, and
    are streaming capable (i.e., can operate on pieces of data to
    minimize the memory footprint).

IO Framework.
    Associated with the data processing pipeline are *sources*, filters
    that initiate the pipeline, and *mappers*, filters that terminate
    the pipeline. The standard examples of sources and mappers are
    *readers* and *writers* respectively. Readers input data (typically
    from a file), and writers output data from the pipeline. *Viewers*
    are another example of mappers.

Spatial Objects.
    Geometric shapes are represented in OTB using the ITK spatial object
    hierarchy. These classes are intended to support modeling of
    anatomical structures in ITK. OTB uses them in order to model
    cartographic elements. Using a common basic interface, the spatial
    objects are capable of representing regions of space in a variety of
    different ways. For example: mesh structures, image masks, and
    implicit equations may be used as the underlying representation
    scheme. Spatial objects are a natural data structure for
    communicating the results of segmentation methods and for
    introducing geometrical priors in both segmentation and registration
    methods.

ITK’s Registration Framework.
    A flexible framework for registration supports four different types
    of registration: image registration, multiresolution registration,
    PDE-based registration, and FEM (finite element method)
    registration.

FEM Framework.
    ITK includes a subsystem for solving general FEM problems, in
    particular non-rigid registration. The FEM package includes mesh
    definition (nodes and elements), loads, and boundary conditions.

Level Set Framework.
    The level set framework is a set of classes for creating filters to
    solve partial differential equations on images using an iterative,
    finite difference update scheme. The level set framework consists of
    finite difference solvers including a sparse level set solver, a
    generic level set segmentation filter, and several specific
    subclasses including threshold, Canny, and Laplacian based methods.

Wrapping.
    ITK uses a unique, powerful system for producing interfaces (i.e.,
    “wrappers”) to interpreted languages such as Tcl and Python. The
    GCC\_XML tool is used to produce an XML description of arbitrarily
    complex C++ code; CSWIG is then used to transform the XML
    description into wrappers using the `SWIG <http://www.swig.org/>`_
    package. OTB does not use this system at present.

Essential System Concepts
-------------------------

[sec:EssentialSystemConcepts]

This section describes some of the core concepts and implementation
features found in ITK and therefore also in OTB.

Generic Programming
~~~~~~~~~~~~~~~~~~~

[sec:GenericProgramming]

Generic programming is a method of organizing libraries consisting of
generic—or reusable—software components . The idea is to make software
that is capable of “plugging together” in an efficient, adaptable
manner. The essential ideas of generic programming are *containers* to
hold data, *iterators* to access the data, and *generic algorithms* that
use containers and iterators to create efficient, fundamental algorithms
such as sorting. Generic programming is implemented in C++ with the
*template* programming mechanism and the use of the STL Standard
Template Library .

C++ templating is a programming technique allowing users to write
software in terms of one or more unknown types . To create executable
code, the user of the software must specify all types (known as
*template instantiation*) and successfully process the code with the
compiler. The may be a native type such as or , or may be a user-defined
type (e.g., ). At compile-time, the compiler makes sure that the
templated types are compatible with the instantiated code and that the
types are supported by the necessary methods and operators.

ITK uses the techniques of generic programming in its implementation.
The advantage of this approach is that an almost unlimited variety of
data types are supported simply by defining the appropriate template
types. For example, in OTB it is possible to create images consisting of
almost any type of pixel. In addition, the type resolution is performed
at compile-time, so the compiler can optimize the code to deliver
maximal performance. The disadvantage of generic programming is that
many compilers still do not support these advanced concepts and cannot
compile OTB. And even if they do, they may produce completely
undecipherable error messages due to even the simplest syntax errors. If
you are not familiar with templated code and generic programming, we
recommend the two books cited above.

Include Files and Class Definitions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[sec:IncludeFiles]

In ITK and OTB classes are defined by a maximum of two files: a header
file and an implementation file— if a non-templated class, and a if a
templated class. The header files contain class declarations and
formatted comments that are used by the Doxygen documentation system to
automatically produce HTML manual pages.

In addition to class headers, there are a few other important header
files.

    is found in the directory and defines standard system-wide macros
    (such as , constants, and other parameters).

    is found in the directory and defines numeric characteristics for
    native types such as its maximum and minimum possible values.

    is found in the and is used to define operating system parameters to
    control the compilation process.

Object Factories
~~~~~~~~~~~~~~~~

[sec:ObjectFactories]

Most classes in OTB are instantiated through an *object factory*
mechanism. That is, rather than using the standard C++ class constructor
and destructor, instances of an OTB class are created with the static
class method. In fact, the constructor and destructor are so it is
generally not possible to construct an OTB instance on the heap. (Note:
this behavior pertains to classes that are derived from . In some cases
the need for speed or reduced memory footprint dictates that a class not
be derived from LightObject and in this case instances may be created on
the heap. An example of such a class is .)

The object factory enables users to control run-time instantiation of
classes by registering one or more factories with . These registered
factories support the method which takes as input the name of a class to
create. The factory can choose to create the class based on a number of
factors including the computer system configuration and environment
variables. For example, in a particular application an OTB user may wish
to deploy their own class implemented using specialized image processing
hardware (i.e., to realize a performance gain). By using the object
factory mechanism, it is possible at run-time to replace the creation of
a particular OTB filter with such a custom class. (Of course, the class
must provide the exact same API as the one it is replacing.) To do this,
the user compiles her class (using the same compiler, build options,
etc.) and inserts the object code into a shared library or DLL. The
library is then placed in a directory referred to by the environment
variable. On instantiation, the object factory will locate the library,
determine that it can create a class of a particular name with the
factory, and use the factory to create the instance. (Note: if the
method cannot find a factory that can create the named class, then the
instantiation of the class falls back to the usual constructor.)

In practice object factories are used mainly (and generally
transparently) by the OTB input/output (IO) classes. For most users the
greatest impact is on the use of the method to create a class. Generally
the method is declared and implemented via the macro found in .

Smart Pointers and Memory Management
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[sec:SmartPointers]

By their nature object-oriented systems represent and operate on data
through a variety of object types, or classes. When a particular class
is instantiated to produce an instance of that class, memory allocation
occurs so that the instance can store data attribute values and method
pointers (i.e., the vtable). This object may then be referenced by other
classes or data structures during normal operation of the program.
Typically during program execution all references to the instance may
disappear at which point the instance must be deleted to recover memory
resources. Knowing when to delete an instance, however, is difficult.
Deleting the instance too soon results in program crashes; deleting it
too late and memory leaks (or excessive memory consumption) will occur.
This process of allocating and releasing memory is known as memory
management.

In ITK, memory management is implemented through reference counting.
This compares to another popular approach—garbage collection—used by
many systems including Java. In reference counting, a count of the
number of references to each instance is kept. When the reference goes
to zero, the object destroys itself. In garbage collection, a background
process sweeps the system identifying instances no longer referenced in
the system and deletes them. The problem with garbage collection is that
the actual point in time at which memory is deleted is variable. This is
unacceptable when an object size may be gigantic (think of a large 3D
volume gigabytes in size). Reference counting deletes memory immediately
(once all references to an object disappear).

Reference counting is implemented through a / member function interface.
All instances of an OTB object have a method invoked on them by any
other object that references an them. The method increments the
instances’ reference count. When the reference to the instance
disappears, a method is invoked on the instance that decrements the
reference count—this is equivalent to an method. When the reference
count returns to zero, the instance is destroyed.

This protocol is greatly simplified by using a helper class called a .
The smart pointer acts like a regular pointer (e.g. supports operators
and ) but automagically performs a when referring to an instance, and an
when it no longer points to the instance. Unlike most other instances in
OTB, SmartPointers can be allocated on the program stack, and are
automatically deleted when the scope that the SmartPointer was created
is closed. As a result, you should *rarely if ever call Register() or
Delete()* in OTB. For example:

::

      MyRegistrationFunction()
        { <----- Start of scope

        // here an interpolator is created and associated to the
        // SmartPointer "interp".
        InterpolatorType::Pointer interp = InterpolatorType::New();

        } <------ End of scope

In this example, reference counted objects are created (with the method)
with a reference count of one. Assignment to the SmartPointer does not
change the reference count. At the end of scope, is destroyed, the
reference count of the actual interpolator object (referred to by ) is
decremented, and if it reaches zero, then the interpolator is also
destroyed.

Note that in ITK SmartPointers are always used to refer to instances of
classes derived from . Method invocations and function calls often
return “real” pointers to instances, but they are immediately assigned
to a SmartPointer. Raw pointers are used for non-LightObject classes
when the need for speed and/or memory demands a smaller, faster class.

Error Handling and Exceptions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[sec:ErrorHandling]

In general, OTB uses exception handling to manage errors during program
execution. Exception handling is a standard part of the C++ language and
generally takes the form as illustrated below:

::

      try
        {
        //...try executing some code here...
        }
      catch ( itk::ExceptionObject exp )
        {
        //...if an exception is thrown catch it here
        }

where a particular class may throw an exceptions as demonstrated below
(this code snippet is taken from :

::

      switch ( sizeof(T) )
        {
        //non-error cases go here followed by error case  
        default:  
          ByteSwapperError e(__FILE__, __LINE__);
          e.SetLocation("SwapBE");
          e.SetDescription("Cannot swap number of bytes requested");
          throw e;
        }

Note that is a subclass of . (In fact in OTB all exceptions should be
derived from .) In this example a special constructor and C++
preprocessor variables and are used to instantiate the exception object
and provide additional information to the user. You can choose to catch
a particular exception and hence a specific OTB error, or you can trap
*any* OTB exception by catching ExceptionObject.

Event Handling
~~~~~~~~~~~~~~

[sec:EventHandling]

Event handling in OTB is implemented using the Subject/Observer design
pattern (sometimes referred to as the Command/Observer design pattern).
In this approach, objects indicate that they are watching for a
particular event—invoked by a particular instance–by registering with
the instance that they are watching. For example, filters in OTB
periodically invoke the . Objects that have registered their interest in
this event are notified when the event occurs. The notification occurs
via an invocation of a command (i.e., function callback, method
invocation, etc.) that is specified during the registration process.
(Note that events in OTB are subclasses of EventObject; look in to
determine which events are available.)

To recap via example: various objects in OTB will invoke specific events
as they execute (from ProcessObject):

::

      this->InvokeEvent( ProgressEvent() );

To watch for such an event, registration is required that associates a
command (e.g., callback function) with the event: method:

::

      unsigned long progressTag = 
        filter->AddObserver(ProgressEvent(), itk::Command*);

When the event occurs, all registered observers are notified via
invocation of the associated method. Note that several subclasses of
Command are available supporting const and non-const member functions as
well as C-style functions. (Look in to find pre-defined subclasses of
Command. If nothing suitable is found, derivation is another
possibility.)

Multi-Threading
~~~~~~~~~~~~~~~

[sec:MultiThreading]

Multithreading is handled in OTB through ITK’s high-level design
abstraction. This approach provides portable multithreading and hides
the complexity of differing thread implementations on the many systems
supported by OTB. For example, the class provides support for
multithreaded execution using on an SGI, or on any platform supporting
POSIX threads.

Multithreading is typically employed by an algorithm during its
execution phase. MultiThreader can be used to execute a single method on
multiple threads, or to specify a method per thread. For example, in the
class (a superclass for most image processing filters) the method uses
the following methods:

::

      multiThreader->SetNumberOfThreads(int);
      multiThreader->SetSingleMethod(ThreadFunctionType, void* data);
      multiThreader->SingleMethodExecute();

In this example each thread invokes the same method. The multithreaded
filter takes care to divide the image into different regions that do not
overlap for write operations.

The general philosophy in ITK regarding thread safety is that accessing
different instances of a class (and its methods) is a thread-safe
operation. Invoking methods on the same instance in different threads is
to be avoided.

Numerics
--------

[sec:Numerics]

OTB; as ITK, uses the VNL numerics library to provide resources for
numerical programming combining the ease of use of packages like
Mathematica and Matlab with the speed of C and the elegance of C++. It
provides a C++ interface to the high-quality Fortran routines made
available in the public domain by numerical analysis researchers. ITK
extends the functionality of VNL by including interface classes between
VNL and ITK proper.

The VNL numerics library includes classes for

Matrices and vectors.
    Standard matrix and vector support and operations on these types.

Specialized matrix and vector classes.
    Several special matrix and vector class with special numerical
    properties are available. Class provides a fast and convenient
    diagonal matrix, while fixed size matrices and vectors allow
    "fast-as-C" computations (see and example subclasses and ).

Matrix decompositions.
    Classes , , and .

Real polynomials.
    Class stores the coefficients of a real polynomial, and provides
    methods of evaluation of the polynomial at any x, while class
    provides a root finder.

Optimization.
    Classes , , , allow optimization of user-supplied functions either
    with or without user-supplied derivatives.

Standardized functions and constants.
    Class defines constants (pi, e, eps...) and simple functions (sqr,
    abs, rnd...). Class is from the ISO standard document, and provides
    a way to access basic limits of a type. For example returns the
    maximum value of a short.

Most VNL routines are implemented as wrappers around the high-quality
Fortran routines that have been developed by the numerical analysis
community over the last forty years and placed in the public domain. The
central repository for these programs is the "netlib" server
http://www.netlib.org/. The National Institute of Standards and
Technology (NIST) provides an excellent search interface to this
repository in its *Guide to Available Mathematical Software (GAMS)* at
http://gams.nist.gov, both as a decision tree and a text search.

ITK also provides additional numerics functionality. A suite of
optimizers, that use VNL under the hood and integrate with the
registration framework are available. A large collection of statistics
functions—not available from VNL—are also provided in the directory. In
addition, a complete finite element (FEM) package is available,
primarily to support the deformable registration in ITK.

Data Representation
-------------------

[sec:DataRepresentationAndAccess]

There are two principal types of data represented in OTB: images and
meshes. This functionality is implemented in the classes Image and Mesh,
both of which are subclasses of . In OTB, data objects are classes that
are meant to be passed around the system and may participate in data
flow pipelines (see Section [sec:DataProcessingPipeline] on page  for
more information).

represents an *n*-dimensional, regular sampling of data. The sampling
direction is parallel to each of the coordinate axes, and the origin of
the sampling, inter-pixel spacing, and the number of samples in each
direction (i.e., image dimension) can be specified. The sample, or
pixel, type in OTB is arbitrary—a template parameter specifies the type
upon template instantiation. (The dimensionality of the image must also
be specified when the image class is instantiated.) The key is that the
pixel type must support certain operations (for example, addition or
difference) if the code is to compile in all cases (for example, to be
processed by a particular filter that uses these operations). In
practice the OTB user will use a C++ simple type (e.g., , ) or a
pre-defined pixel type and will rarely create a new type of pixel class.

One of the important ITK concepts regarding images is that rectangular,
continuous pieces of the image are known as *regions*. Regions are used
to specify which part of an image to process, for example in
multithreading, or which part to hold in memory. In ITK there are three
common types of regions:

#. —the image in its entirety.

#. —the portion of the image retained in memory.

#. —the portion of the region requested by a filter or other class when
   operating on the image.

The class extends the functionalities of the in order to take into
account particular remote sensing features as geographical projections,
etc.

The Mesh class represents an *n*-dimensional, unstructured grid. The
topology of the mesh is represented by a set of *cells* defined by a
type and connectivity list; the connectivity list in turn refers to
points. The geometry of the mesh is defined by the *n*-dimensional
points in combination with associated cell interpolation functions. is
designed as an adaptive representational structure that changes
depending on the operations performed on it. At a minimum, points and
cells are required in order to represent a mesh; but it is possible to
add additional topological information. For example, links from the
points to the cells that use each point can be added; this provides
implicit neighborhood information assuming the implied topology is the
desired one. It is also possible to specify boundary cells explicitly,
to indicate different connectivity from the implied neighborhood
relationships, or to store information on the boundaries of cells.

The mesh is defined in terms of three template parameters: 1) a pixel
type associated with the points, cells, and cell boundaries; 2) the
dimension of the points (which in turn limits the maximum dimension of
the cells); and 3) a “mesh traits” template parameter that specifies the
types of the containers and identifiers used to access the points,
cells, and/or boundaries. By using the mesh traits carefully, it is
possible to create meshes better suited for editing, or those better
suited for “read-only” operations, allowing a trade-off between
representation flexibility, memory, and speed.

Mesh is a subclass of . The PointSet class can be used to represent
point clouds or randomly distributed landmarks, etc. The PointSet class
has no associated topology.

Data Processing Pipeline
------------------------

[sec:DataProcessingPipeline]

While data objects (e.g., images and meshes) are used to represent data,
*process objects* are classes that operate on data objects and may
produce new data objects. Process objects are classed as *sources*,
*filter objects*, or *mappers*. Sources (such as readers) produce data,
filter objects take in data and process it to produce new data, and
mappers accept data for output either to a file or some other system.
Sometimes the term *filter* is used broadly to refer to all three types.

The data processing pipeline ties together data objects (e.g., images
and meshes) and process objects. The pipeline supports an automatic
updating mechanism that causes a filter to execute if and only if its
input or its internal state changes. Further, the data pipeline supports
*streaming*, the ability to automatically break data into smaller
pieces, process the pieces one by one, and reassemble the processed data
into a final result.

Typically data objects and process objects are connected together using
the and methods as follows:

::

      typedef otb::Image<float,2> FloatImage2DType;

      itk::RandomImageSource<FloatImage2DType>::Pointer random;
      random = itk::RandomImageSource<FloatImage2DType>::New();
      random->SetMin(0.0);
      random->SetMax(1.0);

      itk::ShrinkImageFilter<FloatImage2DType,FloatImage2DType>::Pointer shrink;
      shrink = itk::ShrinkImageFilter<FloatImage2DType,FloatImage2DType>::New();
      shrink->SetInput(random->GetOutput());
      shrink->SetShrinkFactors(2);

      otb::ImageFileWriter::Pointer<FloatImage2DType> writer;
      writer = otb::ImageFileWriter::Pointer<FloatImage2DType>::New();
      writer->SetInput (shrink->GetOutput());
      writer->SetFileName( ``test.raw'' );
      writer->Update();

In this example the source object is connected to the , and the shrink
filter is connected to the mapper . When the method is invoked on the
writer, the data processing pipeline causes each of these filters in
order, culminating in writing the final data to a file on disk.

Spatial Objects
---------------

[sec:SpatialObjectsOverview] The ITK spatial object framework supports
the philosophy that the task of image segmentation and registration is
actually the task of object processing. The image is but one medium for
representing objects of interest, and much processing and data analysis
can and should occur at the object level and not based on the medium
used to represent the object.

ITK spatial objects provide a common interface for accessing the
physical location and geometric properties of and the relationship
between objects in a scene that is independent of the form used to
represent those objects. That is, the internal representation maintained
by a spatial object may be a list of points internal to an object, the
surface mesh of the object, a continuous or parametric representation of
the object’s internal points or surfaces, and so forth.

The capabilities provided by the spatial objects framework supports
their use in object segmentation, registration, surface/volume
rendering, and other display and analysis functions. The spatial object
framework extends the concept of a “scene graph” that is common to
computer rendering packages so as to support these new functions. With
the spatial objects framework you can:

#. Specify a spatial object’s parent and children objects. In this way,
   a city may contain roads and those roads can be organized in a tree
   structure.

#. Query if a physical point is inside an object or (optionally) any of
   its children.

#. Request the value and derivatives, at a physical point, of an
   associated intensity function, as specified by an object or
   (optionally) its children.

#. Specify the coordinate transformation that maps a parent object’s
   coordinate system into a child object’s coordinate system.

#. Compute the bounding box of a spatial object and (optionally) its
   children.

#. Query the resolution at which the object was originally computed. For
   example, you can query the resolution (i.e., pixel spacing) of the
   image used to generate a particular instance of a .

Currently implemented types of spatial objects include: Blob, Ellipse,
Group, Image, Line, Surface, and Tube. The object is used to hold a list
of spatial objects that may in turn have children. Each spatial object
can be assigned a color property. Each spatial object type has its own
capabilities. For example, s indicate to what point on their parent tube
they connect.

There are a limited number of spatial objects and their methods in ITK,
but their number is growing and their potential is huge. Using the
nominal spatial object capabilities, methods such as mutual information
registration, can be applied to objects regardless of their internal
representation. By having a common API, the same method can be used to
register a parametric representation of a building with an image or to
register two different segmentations of a particular object in
object-based change detection.
