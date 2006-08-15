CEGUI MeshViewer for OGRE
=========================

This is a simple crossplatform Mesh Viewer for OGRE using CEGUI. It is
mainly being developed in GNU/Linux so it might require some tweaking to
compile properly in other platforms.

Running the Mesh Viewer
-----------------------

Once the program has been compiled, you can find the binary in the src
subdirectory. It is recommended that you run the viewer from src.

The Mesh Viewer is only able to load meshes from the locations
specified in the file resources.cfg, and it also requires a valid
plugins.cfg.

Sample configuration files can be found in src. The included resources.cfg
will load all meshes in the media subdirectory, so for a quick test just
put a .mesh and .material file in media and launch the program.

Note: You should always include media/gui as a resource path for the viewer to
work.
