#!/bin/bash
haxe -cp generator  -lib hl-idl --macro "clip.Generator.generateCpp()"