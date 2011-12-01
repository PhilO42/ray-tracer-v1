#!/bin/bash
make -j
for i in -1.46608 -1.15192 -0.83776 -0.52360 -0.20944 0.10472 0.41888 0.73304 1.04720 1.36136


do
	echo "using t = " $i
	./bin/ray_tracer $i -a
done
