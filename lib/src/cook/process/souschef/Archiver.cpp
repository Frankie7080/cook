#include "cook/process/souschef/Archiver.hpp"
#include "boost/predef.h"
#include "gubg/stream.hpp"

namespace cook { namespace process { namespace souschef {

namespace  {

struct DummyArchiver : public build::Command
{
    std::string name() const override { return "archive"; }
    Result process(const std::list<std::filesystem::path> & input, const std::list<std::filesystem::path> & output) override
    {
        return Result();
    }
};


std::string construct_archive_filename(const model::Recipe & recipe)
{
#if BOOST_OS_WINDOWS
    return gubg::stream([&](auto & os) {os << << recipe.uri().as_relative().string('.') << ".lib"; });
#else
    return gubg::stream([&](auto & os) {os << "lib" << recipe.uri().as_relative().string('.') << ".a"; });
#endif
}

}

Result Archiver::process(model::Recipe & recipe, RecipeFilteredGraph & file_command_graph, const Context & context) const
{
    MSS_BEGIN(Result);

    model::Recipe::Files & files = recipe.files();
    auto & g = file_command_graph;

    auto objects = files.range(LanguageTypePair(Language::Binary, Type::Object));
    if (objects.empty())
    {
        MSS_RC << MESSAGE(Warning, "archive for " << recipe.uri() << " is not created as it contains no object code");
        MSS_RETURN_OK();
    }

    auto archive_vertex = g.add_vertex(archive_command(context));

    // stop the propagation, this file is contained in the library
    for(ingredient::File & object : objects)
    {
        object.set_propagation(Propagation::Private);
        MSS(g.add_edge(g.goc_vertex(object.key()), archive_vertex));
    }

    // create the archive
    const ingredient::File archive = construct_archive_file(recipe, context);
    const LanguageTypePair key(Language::Binary, Type::Library);
    MSG_MSS(files.insert(key,archive).second, Error, "Archive " << archive << " already present in " << recipe.uri());

    // add the link in the execution graph
    MSS(g.add_edge(archive_vertex, g.goc_vertex(archive.key())));

    MSS_END();
}


ingredient::File Archiver::construct_archive_file(model::Recipe &recipe, const Context &context) const
{
    const std::filesystem::path dir = context.dirs().output();
    const std::filesystem::path rel = construct_archive_filename(recipe);

    ingredient::File archive(dir, rel);
    archive.set_overwrite(Overwrite::IfSame);
    archive.set_owner(&recipe);
    archive.set_propagation(Propagation::Public);

    return archive;
}

build::CommandPtr Archiver::archive_command(const Context & context) const
{
    return std::make_shared<DummyArchiver>();
}

} } }
