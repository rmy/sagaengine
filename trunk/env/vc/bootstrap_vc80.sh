#!/bin/sh

cp application_vc80.vctmpl appliaction.vctmpl
cp library_vc80.vctmpl library.vctmpl


cd ../..
cd samples/simple/projects/vc
sh bootstrap.sh
cd ../../../..
cd samples/fly/projects/vc
sh bootstrap.sh
cd ../../../..
cd samples/navmesh/projects/vc/
sh bootstrap.sh
