#ifndef HEADER_cook_generator_Ninja_hpp_ALREADY_INCLUDED
#define HEADER_cook_generator_Ninja_hpp_ALREADY_INCLUDED

#include "cook/generator/Interface.hpp"

namespace cook { namespace generator { 

    class Ninja: public Interface
    {
    public:
        //Interface implementation
        std::string name() const override {return "ninja";}
        Result set_option(const std::string & option);
        bool can_process(const Context & context) const;
        Result process(std::ostream & ofs, const Context & context);
        std::filesystem::path output_filename(const model::Dirs & dirs) const override;

    private:
    };

} } 

#endif
