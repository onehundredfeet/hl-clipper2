package ;

#if eval
class Generator {
	// Put any necessary includes in this string and they will be added to the generated files
	static var HL_INCLUDE = "
#ifdef _WIN32
#pragma warning(disable:4305)
#pragma warning(disable:4244)
#pragma warning(disable:4316)
#endif


#include \"hl-clip.h\"
#include \"polypartition.h\"
using namespace Clipper2Lib;
";
static var JVM_INCLUDE ="
#include \"hl-clip.h\"
#include \"polypartition.h\"
using namespace Clipper2Lib;
";

	static var options = {
		idlFile: "lib/clip/clip.idl",
		target: null,
		packageName: "clipper2",
		nativeLib: "clipper2",
		outputDir: "src",
		includeCode: null,
		autoGC: true
	};

	public static function generateCpp(target = idl.Options.Target.TargetHL) {
		options.target = target;
		options.includeCode = switch (target) {
			case idl.Options.Target.TargetHL: HL_INCLUDE;
			case idl.Options.Target.TargetJVM: JVM_INCLUDE;
			default: "";
		};
		idl.generator.Generate.generateCpp(options);
	}
}
#end
