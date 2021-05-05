#include "cook/process/toolchain/serialize/GCC.hpp"
#include "cook/process/toolchain/serialize/Standard.hpp"
#include <string>

namespace cook { namespace process { namespace toolchain { namespace serialize {

    void gcc_config(std::ostream & oss, GCCVariant gcc_variant, const std::vector<Language> &languages, const std::string &compiler, const std::string &archiver, const std::string & linker)
    {
        standard_config(oss);

        oss << R"%(cook.toolchain.configure(100, "specific toolchain config", fun(e,k,v,b) {
    auto & kv = e.key_values();
    auto & tm = e.translators();

    if (false) {
    } else if (e.element_type == ElementType.Compile) {
        if (false) {
        } else if (k == "ctng.tuple") {
            kv.clear(Part.Cli)
            kv.append(Part.Cli, "${getenv("HOME")}/x-tools/${v}/bin/${v}-gcc")
        } else if (k == "compiler") {
            kv.clear(Part.Cli)
            kv.append(Part.Cli, v)
        } else if (k == "debug_symbols" && v == "true") {
            kv.append(Part.Pre, "-g")
        } else if (k == "optimization" && v == "max_speed") {
            kv.append(Part.Pre, "-O3")
        } else if (k == "config" && v == "rtc") {
            kv.append(Part.Pre, "-fsanitize", "address")
            kv.append(Part.Pre, "-fsanitize", "undefined")
            kv.append(Part.Pre, "-fno-omit-frame-pointer")
        } else if (k == "config" && v == "profile") {
            kv.append(Part.Pre, "-pg")
        } else if (k == "arch" && v == "x86") {
            kv.append(Part.Pre, "-m32")
        } else if (k == "arch" && v == "x64") {
            kv.append(Part.Pre, "-m64")
        } else if (k == "arch" && v == "armv7") {
            kv.append(Part.Pre, "-arch armv7")
            b.add_config("position_independent_code", "true")
        } else if (k == "arch" && v == "arm64") {
            kv.append(Part.Pre, "-arch arm64")
            b.add_config("position_independent_code", "true")
        } else if (k == "arch" && v == "a53") {
            kv.append(Part.Pre, "-mcpu", "cortex-a53")
            kv.append(Part.Pre, "-mfpu", "auto")
            kv.append(Part.Pre, "-funsafe-math-optimizations")
            kv.append(Part.Pre, "-ftree-vectorize")
        } else if (k == "position_independent_code" && v == "true") {
            kv.append(Part.Pre, "-fPIC"))%";
        if (gcc_variant == GCCVariant::Genuine)
        {
        oss << R"%(
        } else if (k == "c++.runtime") {
            if (false) {}
            else if (v == "dynamic") { }
            else if (v == "static") { kv.append(Part.Runtime, "-static-libstdc++") })%";
        }
        oss << R"%(
        } else if (k == "c++.std" && e.language == Language.CXX) {
            kv.append(Part.Pre, "-std", "c++${v}")
        } else if (k == "c.std" && e.language == Language.C) {
            kv.append(Part.Pre, "-std", "c${v}")
        } else if (k == "c++.std" && e.language == Language.ObjectiveCXX) {
            kv.append(Part.Pre, "-std", "c++${v}")
        } else if (k == "c.std" && e.language == Language.ObjectiveC) {
            kv.append(Part.Pre, "-std", "c${v}")
        } else if (k == "sysroot") {
            kv.append(Part.Pre, "--sysroot", v)
        } else {
            return false;
        }
    } else if (e.element_type == ElementType.Link) {
        if (false) {
        } else if (k == "ctng.tuple") {
            kv.clear(Part.Cli)
            kv.append(Part.Cli, "${getenv("HOME")}/x-tools/${v}/bin/${v}-g++")
        } else if (k == "linker") {
            kv.clear(Part.Cli)
            kv.append(Part.Cli, v)
        } else if (k == "arch" && v == "x86") {
            kv.append(Part.Pre, "-m32", "")
        } else if (k == "arch" && v == "x64") {
            kv.append(Part.Pre, "-m64")
        } else if (k == "arch" && v == "armv7") {
            kv.append(Part.Pre, "-arch armv7", "")
        } else if (k == "arch" && v == "arm64") {
            kv.append(Part.Pre, "-arch arm64", "")
        } else if (k == "config" && v == "rtc") {
            kv.append(Part.Pre, "-fsanitize", "address") 
            kv.append(Part.Pre, "-fsanitize", "undefined")
        } else if (k == "config" && v == "profile") {
            kv.append(Part.Pre, "-pg", ""))%";
        if (gcc_variant == GCCVariant::Genuine)
        {
        oss << R"%(
        } else if (k == "c++.runtime") {
            if (false) {}
            else if (v == "dynamic") { }
            else if (v == "static") { kv.append(Part.Runtime, "-static-libstdc++") })%";
        }
        oss << R"%(
        } else if (k == "c++.std") {
            kv.append(Part.Pre, "-std", "c++${v}") 
        } else if (k == "c.std") {
            kv.append(Part.Pre, "-std", "c${v}")
        } else if (k == "sysroot") {
            kv.append(Part.Pre, "--sysroot", v)
        } else {
            return false
        }
    } else if (e.element_type == ElementType.Archive) {
        if (false) {
        } else if (k == "ctng.tuple") {
            kv.clear(Part.Cli)
            kv.append(Part.Cli, "${getenv("HOME")}/x-tools/${v}/bin/${v}-ar")
        } else if (k == "archiver") {
            kv.clear(Part.Cli)
            kv.append(Part.Cli, v)
        } else {
            return false
        }
    } else {
        return false
    }
    return true
})

)%";

        for (auto language: languages)
        {
            oss << "{" << std::endl;
            oss << "    var compiler = cook.toolchain.element(ElementType.Compile, Language." << language << ", TargetType.Object)" << std::endl;
            oss << R"%(
    var & kv = compiler.key_values()
    var & tm = compiler.translators()

    tm[Part.Cli]            = fun(k,v) { return k }
    tm[Part.Pre]            = fun(k,v) { if (v.empty) { return k } else { return "${k}=${v}" } }
    tm[Part.Runtime]        = fun(k,v) { return k }
    tm[Part.Output]         = fun(k,v) { return "-o ${k}" }
    tm[Part.Input]          = fun(k,v) { return k }
    tm[Part.DepFile]        = fun(k,v) { return "-MMD -MF ${k}" }
    tm[Part.Define]         = fun(k,v) { if (v.empty) { return "-D${k}" } else { return "-D${k}=${v}" } }
    tm[Part.IncludePath]    = fun(k,v) { if (k.empty) { return "-I./" } else { return "-I${k}" } }
    tm[Part.ForceInclude]   = fun(k,v) { return "-include ${k}" }

)%";
            oss << "    kv.append(Part.Cli, \"" << compiler << "\")" << std::endl;
            oss << "    kv.append(Part.Pre, \"-c\")" << std::endl;
            switch (language)
            {
                case Language::C            : oss << "    kv.append(Part.Pre, \"-x c\")" << std::endl; break;
                case Language::CXX          : oss << "    kv.append(Part.Pre, \"-x c++\")" << std::endl; break;
                case Language::ObjectiveC   : oss << "    kv.append(Part.Pre, \"-x objective-c\")" << std::endl; break;
                case Language::ObjectiveCXX : oss << "    kv.append(Part.Pre, \"-x objective-c++\")" << std::endl; break;
                case Language::ASM          : oss << "    kv.append(Part.Pre, \"-x assembler\")" << std::endl; break;
                default: break;
            }
            oss << "}" << std::endl;
        }

oss << R"%(

for(s : [TargetType.SharedLibrary, TargetType.Plugin, TargetType.Executable]){
    var linker = cook.toolchain.element(ElementType.Link, Language.Binary, s)
    var & kv = linker.key_values()
    var & tm = linker.translators()
    
    tm[Part.Cli]            = fun(k,v) { return k }
    tm[Part.Pre]            = fun(k,v) { if (v.empty) { return k } else { return "${k}=${v}" } }
    tm[Part.Runtime]        = fun(k,v) { return k }
    tm[Part.Response]       = fun(k,v) { return "@${k}" }
    tm[Part.Output]         = fun(k,v) { return "-o ${k}" }
    tm[Part.Input]          = fun(k,v) { return k }
    tm[Part.Library]        = fun(k,v) { return "-l${k}" }
    tm[Part.LibraryPath]    = fun(k,v) { if (k.empty) { return "-L./" } else { return "-L${k}" } }
    tm[Part.ForceInclude]   = fun(k,v) { return "-include ${k}" }
    if(my(OS) == OS.MacOS) {
        tm[Part.Framework]      = fun(k,v) { return "-framework ${k}" }
        tm[Part.FrameworkPath]  = fun(k,v) { if (k.empty) { return "-F./" } else { return "-F${k}" } }
    }
)%";
            oss << "    kv.append(Part.Cli, \"" << linker << "\")" << std::endl;
oss << R"%(
    if (s == TargetType.SharedLibrary) {
        kv.append(Part.Pre, "-shared")
    } else if (s == TargetType.Plugin) {
        if (my(OS) == OS.MacOS) {
            kv.append(Part.Pre, "-bundle")
        } else {
            kv.append(Part.Pre, "-shared")
        }       
    }
}

{
    var archiver = cook.toolchain.element(ElementType.Archive, Language.Binary, TargetType.Archive)
    var & kv = archiver.key_values()
    var & tm = archiver.translators()
)%";
            oss << "    kv.append(Part.Cli, \"" << archiver << "\")" << std::endl;
oss << R"%(
    tm[Part.Cli]            = fun(k,v) { return k }
    tm[Part.Pre]            = fun(k,v) { return k }
    if (my(OS) != OS.MacOS) {
        tm[Part.Response]       = fun(k,v) { return "@${k}" }
    }
    tm[Part.Output]         = fun(k,v) { return k }
    tm[Part.Input]          = fun(k,v) { return k }
    kv.append(Part.Pre, "crs")
}
)%";

    }

} } } }
