#include "cook/chai/Recipe.hpp"
#include "cook/chai/Context.hpp"
#include "gubg/chai/Module.hpp"

namespace cook { namespace chai {

Recipe::Recipe(model::Recipe * recipe, Context *context, Logger *logger)
    : recipe_(recipe),
      context_(context),
      logger_(logger),
      data_(from_any(recipe->user_data()))
{
}

void Recipe::set_type(Type type)
{
    recipe_->set_type(type);
}

void Recipe::set_working_directory(const std::string & dir)
{
    recipe_->set_working_directory(dir);
}

void Recipe::add(const std::string & dir, const std::string & pattern)
{
    model::GlobInfo info;
    info.dir = dir;
    info.pattern = pattern;

    recipe_->add_globber(info);
}

void Recipe::depends_on(const std::string & dependency)
{
    std::pair<model::Uri, bool> p = model::Uri::recipe_uri(dependency);
    if (!p.second)
        logger_->LOG(Error, "bad uri");

    recipe_->add_dependency(p.first);
}


void Recipe::library(const std::string & library)
{
    auto lib = ingredient::File("", library);
    lib.set_propagation(Propagation::Public);

    recipe_->files().insert(LanguageTypePair(Language::Binary, Type::Library), lib);
}

void Recipe::library_path(const std::string & path)
{
    auto lib_path = ingredient::File(path, "");
    lib_path.set_propagation(Propagation::Public);

    recipe_->files().insert(LanguageTypePair(Language::Binary, Type::LibraryPath), lib_path);
}

void Recipe::include_path(const std::string & path)
{
    auto inc_path = ingredient::File(path, "");
    inc_path.set_propagation(Propagation::Public);

    recipe_->files().insert(LanguageTypePair(Language::CXX, Type::IncludePath), inc_path);
}

gubg::chai::ModulePtr recipe_module()
{
    gubg::chai::ModulePtr ptr = std::make_unique<chaiscript::Module>();

    ptr->add(chaiscript::user_type<Recipe>(), "Recipe");
    ptr->add(chaiscript::fun(&Recipe::add), "add");
    ptr->add(chaiscript::fun(&Recipe::depends_on), "depends_on");
    ptr->add(chaiscript::fun(&Recipe::set_type), "set_type");
    ptr->add(chaiscript::fun(&Recipe::library), "library");
    ptr->add(chaiscript::fun(&Recipe::library_path), "library_path");
    ptr->add(chaiscript::fun(&Recipe::include_path), "include_path");
    ptr->add(chaiscript::fun(&Recipe::data), "data");

    return ptr;
}


} }
