#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <array>
#include <string>
#include <unordered_map>

#include "NodeSprite.h"

constexpr uint32_t NUMBER_OF_TEXTURE_TYPES = 47;
constexpr uint32_t NUMBER_OF_EDGES_IN_A_ROW = 11;
constexpr uint32_t NUMBER_OF_EDGES_IN_A_COLUMN = 11;
constexpr uint32_t NUMBER_OF_NODES_IN_A_ROW = 12;
constexpr uint32_t NUMBER_OF_NODES_IN_A_COLUMN = 6;
constexpr uint32_t NUMBER_OF_RESOURCE_TYPES = 5;

enum class ResourceType;
enum class StructureType;

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

    DESERT_RESOURCE,
    WHEAT_RESOURCE,
    CLAY_RESOURCE,
    SHEEP_RESOURCE,
    TREE_RESOURCE,
    STONE_RESOURCE,

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

    void start_game();
    void create_catan_board(const std::string& board_data);
    void update_board(const std::string& board_data);
    void update_dices(uint32_t first_dice, uint32_t second_dices);
    void update_available_resources(const std::unordered_map<ResourceType, uint32_t>& resources);

private:

    void initialize_textures();

    void set_background_image();
    sf::Vector2f get_background_image_scale(const sf::Texture& background) const;
    
    void initialize_board_resources(const std::string& resources_data);
    void initialize_resource_type(uint32_t resource_index, uint32_t x, uint32_t y, ResourceType resource_type);
    void initialize_resource_type_sprite(uint32_t resource_index, uint32_t x, uint32_t y, TextureTypes resource_type);
    void initialize_resource_number(uint32_t resource_index, uint32_t x, uint32_t y, uint32_t resource_number);
    void initialize_resource_number_sprite(uint32_t resource_index, uint32_t x, uint32_t y, TextureTypes resource_number);
    void initialize_board_robber(const std::string& robber_location);
    void initialize_dices();
    void initialize_available_resources();

    void fetch_structures(const std::string& structures);
    void fetch_edges(const std::string& edges);
    void set_robber_position(const std::string& robber_location);

    void upgrade_settlement_to_city(uint32_t x, uint32_t y, uint32_t player_number);
    void add_new_settlement(uint32_t x, uint32_t y, uint32_t player_number);
    void initialize_structure_position(NodeSprite& structure, uint32_t x, uint32_t y);
    void add_new_edge(uint32_t x, uint32_t y, uint32_t player_number);
    void initialize_edge_position(sf::Sprite& edge, uint32_t x, uint32_t y);

    void render_loop();
    void draw();

private:
    sf::RenderWindow m_window;
    sf::Font m_font;
    std::array<sf::Texture, NUMBER_OF_TEXTURE_TYPES> m_textures;

    sf::Sprite m_background;
    sf::Sprite m_robber;
    sf::Sprite m_dice_1;
    sf::Sprite m_dice_2;

    std::array<sf::Sprite, NUMBER_OF_RESOURCE_TYPES> m_available_resources_images;
    std::array<sf::Text, NUMBER_OF_RESOURCE_TYPES> m_available_resources_texts;

    std::vector<sf::Sprite> m_board_resources;   
    std::vector<sf::Sprite> m_board_resources_numbers;

    std::array<std::array<NodeSprite, NUMBER_OF_NODES_IN_A_ROW>, NUMBER_OF_NODES_IN_A_COLUMN> m_board_structures;
    std::array<std::array<sf::Sprite, NUMBER_OF_EDGES_IN_A_ROW>, NUMBER_OF_EDGES_IN_A_COLUMN> m_board_edges;
};
