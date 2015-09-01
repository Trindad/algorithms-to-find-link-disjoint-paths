#!/bin/bash

for f in Testes/*.txt
do
	echo $f
	./b $f
done