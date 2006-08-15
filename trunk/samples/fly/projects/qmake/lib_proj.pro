!include( env.pro ) {
    include( missing.pro )
}
!include( $${SAGAENGINE}/samples/fly/projects/qmake/proj.pro ) {
    error( "You must link proj.pro to proj_debug.pro or proj_release.pro in your project directory" )
}
