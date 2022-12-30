package clip;


typedef Native = haxe.macro.MacroType<[webidl.Module.build({ idlFile : "clip/clip.idl", chopPrefix : "rc", autoGC : true, nativeLib : "clip" })]>;
