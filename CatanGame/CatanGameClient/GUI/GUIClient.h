#pragma once

#include <SFML/Graphics.hpp>

enum class ResourceType;

#include <vector>
#include <string>

enum class TextureTypes {
    BACKGROUND = 0,

    NUMBER_2,
    NUMBER_3,
    NUMBER_4,
    NUMBER_5,
    NUMBER_6,
    NUMBER_8,
    NUMBER_9,
    NUMBER_10,
    NUMBER_11,
    NUMBER_12,

    SHEEP,
    CLAY,
    DESERT,
    WHEAT,
    STONE,
    TREE,

    DICE_1,
    DICE_2,
    DICE_3,
    DICE_4,
    DICE_5,
    DICE_6,

    SHEEP_RESOURCE,
    CLAY_RESOURCE,
    DESERT_RESOURCE,
    WHEAT_RESOURCE,
    STONE_RESOURCE,
    TREE_RESOURCE,

    KNIHT_CARD,
    MONIPOLY_CARD,
    PLANTY_YEAR_CARD,
    ROAD_CARD,
    POINT_CARD,

    EDGE_RED,
    SETTLEMENT_RED,
    CITY_RED,
    EDGE_GREEN,
    SETTLEMENT_GREEN,
    CITY_GREEN,
    EDGE_YELLOW,
    SETTLEMENT_YELLOW,
    CITY_YELLOW,
    EDGE_BLUE,
    SETTLEMENT_BLUE,
    CITY_BLUE,

    ROBBER,
};

class GUIClient
{
public:
    GUIClient();
    ~GUIClient();
    
    void start_game();
    void create_board(const std::string& board_data);

private:
    void initialize_textures();
    void set_background_image();
    sf::Vector2f get_background_image_scale(const sf::Texture& background) const;
    void render_loop();

    void initialize_board_resources(const std::string& resources_data);
    void initialize_resource_type(uint32_t resource_index, uint32_t x, uint32_t y, ResourceType resource_type);
    void initialize_resource_type_sprite(uint32_t resource_index, uint32_t x, uint32_t y, TextureTypes resource_type);
    void initialize_resource_number(uint32_t resource_index, uint32_t x, uint32_t y, uint32_t resource_number);
    void initialize_resource_number_sprite(uint32_t resource_index, uint32_t x, uint32_t y, TextureTypes resource_number);

private:
    sf::RenderWindow m_window;
    std::vector<sf::Texture> m_textures;
    sf::Sprite m_background;
    std::vector<sf::Sprite> m_board_resources;   
    std::vector<sf::Sprite> m_board_resources_numbers;
};
