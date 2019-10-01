#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

namespace engine
{
    class shader;

    struct character
    {
        //TODO fix comments.. should be before  the variable not after. (i.e. /// \brief blah blah)
        uint32_t   texture_id;  // ID handle of the glyph texture
        glm::ivec2 size;       // Size of glyph
        glm::ivec2 bearing;    // Offset from baseline to left/top of glyph
        uint32_t   advance;    // Offset to advance to next glyph
    };

    // TODO: Consider renaming to text.
    class text_manager
    {
    public:
        text_manager();
        ~text_manager();

        // TODO: no more than 80 - 100 char per line. FIX
        void render_text(engine::ref<engine::shader>, std::string text, float x, float y, float scale, glm::vec3 color);
        static ref<text_manager> create();

    private:
        // TODO: use vertex_array class instead like in mesh
        uint32_t VAO, VBO;
        FT_Library ft;
        std::map<char, character> characters;
        void init();
    };
}

