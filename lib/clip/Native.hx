package clip;

typedef Native = haxe.macro.MacroType<[
	idl.Module.build({
		idlFile: "clip/clip.idl",
		target: #if hl "hl" #elseif (java || jvm) "jvm" #else "error" #end,
		packageName: "clip",
		autoGC: true,
		nativeLib: "clip"
	})
]>;
