#include "GUIClient.h"

#include <string_view>
#include <iostream>

#include "StringUtils/StringUtils.h"
#include "CatanClient/CatanClient.h"

constexpr uint32_t NUMBER_OF_TEXTURE_TYPES = 47;
constexpr uint32_t NUMBER_OF_RESOURCES_IN_BOARD = 19;
constexpr std::string_view WINDOW_NAME = "Eli's Catan";

constexpr float WINDOW_HIGHT = 600.0f;
constexpr float WINDOW_WIDTH = 800.0f;
constexpr float BACKGROUND_WIDTH = 1800.0f;
constexpr float BACKGROUND_HIGHT = 1581.0f;
constexpr float RESOURCE_HIGHT = 1074.0f;
constexpr float RESOURCE_WIDTH = 922.0f;
constexpr float RESOURCE_HIGHT_IN_BACKGROUND = 309.0f;
constexpr float RESOURCE_WIDTH_IN_BACKGROUND = 270.0f;
constexpr float FIRST_POSITION_OF_RESOURCE_X = 230.0f;
constexpr float FIRST_POSITION_OF_RESOURCE_Y = 171.0f;
constexpr float RESOURCE_SCALE_X = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH / RESOURCE_WIDTH;
constexpr float RESOURCE_SCALE_Y = WINDOW_HIGHT * RESOURCE_HIGHT_IN_BACKGROUND / BACKGROUND_HIGHT / RESOURCE_HIGHT;
constexpr float FIRST_POSITION_OF_EDGE_X = 240.0f;
constexpr float FIRST_POSITION_OF_EDGE_Y = 270.0f;
constexpr float FIRST_POSITION_OF_STRUCTURE_X = 215.0f;
constexpr float FIRST_POSITION_OF_STRUCTURE_Y = 190.0f;

constexpr uint32_t NONE_PLAYER = 5;

GUIClient::GUIClient() :
    m_textures(NUMBER_OF_TEXTURE_TYPES)
{
    m_font.loadFromFile("Fonts\\sansation.ttf");
    for (size_t i = 0; i < m_edges_positions.size(); i++) {
        for (size_t j = 0; j < m_edges_positions[i].size(); j++)
        {
            m_edges_positions[i][j] = false;
        }
    }
    for (size_t i = 0; i < m_nodes_positions.size(); i++) {
        for (size_t j = 0; j < m_nodes_positions[i].size(); j++)
        {
            m_nodes_positions[i][j] = StructureType::NONE;
        }
    }
    m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HIGHT), WINDOW_NAME.data());
    initialize_textures();
    set_background_image();
    initialize_available_resources();
}

void GUIClient::initialize_textures() {
    // background
    m_textures[static_cast<uint32_t>(TextureTypes::BACKGROUND)].loadFromFile("Images/empty_game_board.png");
    
    // resources
    m_textures[static_cast<uint32_t>(TextureTypes::DESERT)].loadFromFile("Images/resources/desert.png");
    m_textures[static_cast<uint32_t>(TextureTypes::CLAY)].loadFromFile("Images/resources/clay.png");
    m_textures[static_cast<uint32_t>(TextureTypes::TREE)].loadFromFile("Images/resources/tree.png");
    m_textures[static_cast<uint32_t>(TextureTypes::STONE)].loadFromFile("Images/resources/stone.png");
    m_textures[static_cast<uint32_t>(TextureTypes::WHEAT)].loadFromFile("Images/resources/wheat.png");
    m_textures[static_cast<uint32_t>(TextureTypes::SHEEP)].loadFromFile("Images/resources/sheep.png");
    
    m_textures[static_cast<uint32_t>(TextureTypes::CLAY_RESOURCE)].loadFromFile("Images/resources_types/clay.jpg");
    m_textures[static_cast<uint32_t>(TextureTypes::TREE_RESOURCE)].loadFromFile("Images/resources_types/tree.jpg");
    m_textures[static_cast<uint32_t>(TextureTypes::STONE_RESOURCE)].loadFromFile("Images/resources_types/stone.jpg");
    m_textures[static_cast<uint32_t>(TextureTypes::WHEAT_RESOURCE)].loadFromFile("Images/resources_types/wheat.jpg");
    m_textures[static_cast<uint32_t>(TextureTypes::SHEEP_RESOURCE)].loadFromFile("Images/resources_types/sheep.jpg");

    //numbers
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_2)].loadFromFile("Images/numbers/2.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_3)].loadFromFile("Images/numbers/3.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_4)].loadFromFile("Images/numbers/4.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_5)].loadFromFile("Images/numbers/5.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_6)].loadFromFile("Images/numbers/6.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_8)].loadFromFile("Images/numbers/8.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_9)].loadFromFile("Images/numbers/9.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_10)].loadFromFile("Images/numbers/10.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_11)].loadFromFile("Images/numbers/11.tga");
    m_textures[static_cast<uint32_t>(TextureTypes::NUMBER_12)].loadFromFile("Images/numbers/12.tga");

    //dices
    m_textures[static_cast<uint32_t>(TextureTypes::DICE_1)].loadFromFile("Images/dices/dice1.jpg");
    m_textures[static_cast<uint32_t>(TextureTypes::DICE_2)].loadFromFile("Images/dices/dice2.jpg");
    m_textures[static_cast<uint32_t>(TextureTypes::DICE_3)].loadFromFile("Images/dices/dice3.jpg");
    m_textures[static_cast<uint32_t>(TextureTypes::DICE_4)].loadFromFile("Images/dices/dice4.jpg");
    m_textures[static_cast<uint32_t>(TextureTypes::DICE_5)].loadFromFile("Images/dices/dice5.jpg");
    m_textures[static_cast<uint32_t>(TextureTypes::DICE_6)].loadFromFile("Images/dices/dice6.jpg");

    //objects
    m_textures[static_cast<uint32_t>(TextureTypes::ROBBER)].loadFromFile("Images/objects/catan-robber.png");
    m_textures[static_cast<uint32_t>(TextureTypes::EDGE_BLUE)].loadFromFile("Images/objects/edge_blue.png");
    m_textures[static_cast<uint32_t>(TextureTypes::EDGE_GREEN)].loadFromFile("Images/objects/edge_green.png");
    m_textures[static_cast<uint32_t>(TextureTypes::EDGE_YELLOW)].loadFromFile("Images/objects/edge_yellow.png");
    m_textures[static_cast<uint32_t>(TextureTypes::EDGE_RED)].loadFromFile("Images/objects/edge_red.png");
   
    m_textures[static_cast<uint32_t>(TextureTypes::SETTLEMENT_BLUE)].loadFromFile("Images/objects/settlement_blue.png");
    m_textures[static_cast<uint32_t>(TextureTypes::SETTLEMENT_GREEN)].loadFromFile("Images/objects/settlement_green.jpg");
    m_textures[static_cast<uint32_t>(TextureTypes::SETTLEMENT_YELLOW)].loadFromFile("Images/objects/settlement_yellow.png");
    m_textures[static_cast<uint32_t>(TextureTypes::SETTLEMENT_RED)].loadFromFile("Images/objects/settlement_red.png");
    
    m_textures[static_cast<uint32_t>(TextureTypes::CITY_BLUE)].loadFromFile("Images/objects/city_blue.png");
    m_textures[static_cast<uint32_t>(TextureTypes::CITY_GREEN)].loadFromFile("Images/objects/city_green.png");
    m_textures[static_cast<uint32_t>(TextureTypes::CITY_YELLOW)].loadFromFile("Images/objects/city_yellow.png");
    m_textures[static_cast<uint32_t>(TextureTypes::CITY_RED)].loadFromFile("Images/objects/city_red.png");

}

void GUIClient::set_background_image() {
    m_background.setTexture(m_textures.at(static_cast<uint32_t>(TextureTypes::BACKGROUND)));
    m_background.setPosition(sf::Vector2f(0.0f, 0.0f));

    m_background.setScale(get_background_image_scale(m_textures.at(static_cast<uint32_t>(TextureTypes::BACKGROUND))));
}

sf::Vector2f GUIClient::get_background_image_scale(const sf::Texture& background) const {

    float x;
    float y;

    x = static_cast<float>(m_window.getSize().x);
    x /= background.getSize().x;

    y = static_cast<float>(m_window.getSize().y);
    y /= background.getSize().y;

    return sf::Vector2f(x, y);
}

void GUIClient::initialize_board_resources(const std::string& resources_data) {

    bool pos[] = {
        0, 0, 1, 1, 1,
        0, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 0,
        1, 1, 1, 0, 0
    };

    m_board_resources.resize(NUMBER_OF_RESOURCES_IN_BOARD);
    m_board_resources_numbers.resize(NUMBER_OF_RESOURCES_IN_BOARD);
    uint32_t current_pos = 0;
    uint32_t current_resource = 0;
    for (auto resource_data : split(resources_data, ";")) {
        auto resource_type = static_cast<ResourceType>(atoi(split(resource_data, ",")[0].c_str()));
        uint32_t resource_number = atoi(split(resource_data, ",")[1].c_str());
        if (pos[current_pos]) {
            uint32_t i = current_pos / 5;
            uint32_t j = current_pos % 5;
            initialize_resource_type(current_resource, j, i, resource_type);
            initialize_resource_number(current_resource, j, i, resource_number);
            current_resource++;
        }
        current_pos++;
    }
}

void GUIClient::initialize_resource_type(uint32_t resource_index, uint32_t x, uint32_t y, ResourceType resource_type) {
    switch (resource_type)
    {
    case ResourceType::CLAY:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::CLAY);
        break;
    case ResourceType::SHEEP:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::SHEEP);
        break;
    case ResourceType::WHEAT:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::WHEAT);
        break;
    case ResourceType::NONE:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::DESERT);
        break;
    case ResourceType::STONE:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::STONE);
        break;
    case ResourceType::TREE:
        initialize_resource_type_sprite(resource_index, x, y, TextureTypes::TREE);
        break;
    default:
        break;
    }
}

void GUIClient::initialize_resource_type_sprite(uint32_t resource_index, uint32_t x, uint32_t y, TextureTypes resource_type) {
    m_board_resources[resource_index].setTexture(m_textures[static_cast<uint32_t>(resource_type)]);

    m_board_resources[resource_index].setScale(RESOURCE_SCALE_X, RESOURCE_SCALE_Y);

    auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
    auto size_of_resource_y = WINDOW_HIGHT * RESOURCE_HIGHT_IN_BACKGROUND / BACKGROUND_HIGHT;

    float first_position_in_board_x = FIRST_POSITION_OF_RESOURCE_X * WINDOW_WIDTH / BACKGROUND_WIDTH;
    float first_position_in_board_y = FIRST_POSITION_OF_RESOURCE_Y * WINDOW_HIGHT / BACKGROUND_HIGHT;

    auto x1 = first_position_in_board_x + (x - 1.0f + 0.5f * y) * size_of_resource_x;
    auto y1 = first_position_in_board_y + y * size_of_resource_y * 0.757f;

    m_board_resources[resource_index].setPosition(x1, y1);
}

void GUIClient::initialize_resource_number(uint32_t resource_index, uint32_t x, uint32_t y, uint32_t resource_number) {
    switch (resource_number)
    {
    case 0:
        break;
    case 2:
        initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_2);
        break;
    case 3:
        initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_3);
        break;
    case 4:
        initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_4);
        break;
    case 5:
        initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_5);
        break;
    case 6:
        initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_6);
        break;
    case 8:
        initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_8);
        break;
    case 9:
        initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_9);
        break;
    case 10:
        initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_10);
        break;
    case 11:
        initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_11);
        break;
    case 12:
        initialize_resource_number_sprite(resource_index, x, y, TextureTypes::NUMBER_12);
        break;
    default:
        break;
    }
}

void GUIClient::initialize_resource_number_sprite(uint32_t resource_index, uint32_t x, uint32_t y, TextureTypes resource_number) {
    m_board_resources_numbers[resource_index].setTexture(m_textures[static_cast<uint32_t>(resource_number)]);

    m_board_resources_numbers[resource_index].setScale(RESOURCE_SCALE_X - 0.05f, RESOURCE_SCALE_Y - 0.05f);

    auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
    auto size_of_resource_y = WINDOW_HIGHT * RESOURCE_HIGHT_IN_BACKGROUND / BACKGROUND_HIGHT;

    float first_position_in_board_x = FIRST_POSITION_OF_RESOURCE_X * WINDOW_WIDTH / BACKGROUND_WIDTH;
    float first_position_in_board_y = FIRST_POSITION_OF_RESOURCE_Y * WINDOW_HIGHT / BACKGROUND_HIGHT;

    auto x1 = first_position_in_board_x + (x - 1.0f + 0.5f * y) * size_of_resource_x;
    auto y1 = first_position_in_board_y + y * size_of_resource_y * 0.757f;

    m_board_resources_numbers[resource_index].setPosition(x1 + 35.0f, y1 + 40.0f);
}

void GUIClient::initialize_board_robber(const std::string& robber_location) {
    uint32_t x = stoi(split(robber_location, ",")[0]);
    uint32_t y = stoi(split(robber_location, ",")[1]);

    m_robber.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::ROBBER)]);

    m_robber.setScale(RESOURCE_SCALE_X + 0.1f, RESOURCE_SCALE_Y + 0.1f);

    auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
    auto size_of_resource_y = WINDOW_HIGHT * RESOURCE_HIGHT_IN_BACKGROUND / BACKGROUND_HIGHT;

    float first_position_in_board_x = FIRST_POSITION_OF_RESOURCE_X * WINDOW_WIDTH / BACKGROUND_WIDTH;
    float first_position_in_board_y = FIRST_POSITION_OF_RESOURCE_Y * WINDOW_HIGHT / BACKGROUND_HIGHT;

    auto x1 = first_position_in_board_x + (x - 1.0f + 0.5f * y) * size_of_resource_x;
    auto y1 = first_position_in_board_y + y * size_of_resource_y * 0.757f;

    m_robber.setPosition(x1 + 35.0f, y1 + 40.0f);
}

void GUIClient::initialize_dices() {
    m_dice_1.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::DICE_1)]);
    m_dice_2.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::DICE_1)]);

    m_dice_1.setScale(0.3f, 0.3f);
    m_dice_2.setScale(0.3f, 0.3f);

    m_dice_1.setPosition(0, 0);
    m_dice_2.setPosition(50, 0);
}

void GUIClient::initialize_available_resources() {
    for (size_t i = 0; i < m_available_resources_images.size(); i++) {
        m_available_resources_images[i].setTexture(m_textures[static_cast<uint32_t>(TextureTypes::WHEAT_RESOURCE) + i]);
        m_available_resources_images[i].setPosition(m_window.getSize().x - 80.0f, 20.0f + i * 25);
        m_available_resources_images[i].setScale(0.5f, 0.5f);
        
        m_available_resources_texts[i].setFont(m_font);
        m_available_resources_texts[i].setFillColor(sf::Color::Black);
        m_available_resources_texts[i].setString("0");
        m_available_resources_texts[i].setPosition(m_window.getSize().x - 30.0f, 15.0f + i * 25);
        m_available_resources_texts[i].setCharacterSize(24);
        m_available_resources_texts[i].setStyle(sf::Text::Bold);
    }
}

void GUIClient::start_game() {
    render_loop();
}

void GUIClient::create_catan_board(const std::string& board_data) {

    auto all_data = split(board_data, "\n");
    initialize_board_resources(all_data[0]);
    initialize_board_robber(all_data[3]);
    initialize_dices();

}

void GUIClient::update_board(const std::string& board_data) {

    auto all_data = split(board_data, "\n");
    fetch_structures(all_data[1]);
    fetch_edges(all_data[2]);
    set_robber_position(all_data[3]);
}

void GUIClient::fetch_structures(const std::string& structures) {
    uint32_t x = 0;
    uint32_t y = -1;

    auto current_structure = split(structures, ";");
    for (const auto& structure : current_structure) {
        if (x % NUMBER_OF_NODES_IN_A_ROW == 0) {
            y++;
            x = 0;
        }
        auto structure_data = split(structure, ",");
        uint32_t structure_player = std::stoi(structure_data[1]);
        StructureType structure_type = static_cast<StructureType>(std::stoi(structure_data[0]));
        if (structure_type == StructureType::NONE || structure_type == m_nodes_positions[y][x]) {
            x++;
            continue;
        }
        if (structure_type == StructureType::SETTLEMENT) {
            add_new_settlement(x, y, structure_player);
        }
        else {
            // TODO set the texture of the structure to city
        }
        m_nodes_positions[y][x] = structure_type;
        x++;
    }
}

void GUIClient::add_new_settlement(uint32_t x, uint32_t y, uint32_t player_number) {
    sf::Sprite structure;
    switch (static_cast<PlayerType>(player_number))
    {
    case PlayerType::BLUE:
        structure.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::SETTLEMENT_BLUE)]);
        break;
    case PlayerType::GREEN:
        structure.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::SETTLEMENT_GREEN)]);
        break;
    case PlayerType::RED:
        structure.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::SETTLEMENT_RED)]);
        break;
    case PlayerType::YELLOW:
        structure.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::SETTLEMENT_YELLOW)]); 
        break;
    default:
        break;
    }

    structure.setScale(0.09f, 0.09f);
    initialize_structure_position(structure, x, y);

    m_board_structures.push_back(structure);
}

void GUIClient::initialize_structure_position(sf::Sprite& structure, uint32_t x, uint32_t y) {

    // TODO fix this function - the position is not accurate
    auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
    auto size_of_resource_y = WINDOW_HIGHT * RESOURCE_HIGHT_IN_BACKGROUND / BACKGROUND_HIGHT;
    float shift_right = NUMBER_OF_EDGES_IN_A_COLUMN / 2;
    shift_right -= y;
    shift_right = std::max(shift_right, 0.0f);
    shift_right = x - shift_right;
    shift_right /= 2;

    float temp = NUMBER_OF_EDGES_IN_A_COLUMN / 2.0f - y;
    temp = std::abs(std::floor(temp)) / 4;
    shift_right += temp;

    auto x1 = (FIRST_POSITION_OF_STRUCTURE_X * WINDOW_WIDTH / BACKGROUND_WIDTH) + size_of_resource_x * shift_right;
    auto y1 = (FIRST_POSITION_OF_STRUCTURE_Y * WINDOW_HIGHT / BACKGROUND_HIGHT) + size_of_resource_y * (y * 0.757f);
    if (y == 0) {
        x--;
    }
    if (x % 2 == 1) {
        y1 += 9.0f;
    }
    else {
        y1 -= 9.0f;
    }
    structure.setPosition(x1, y1);
}

void GUIClient::fetch_edges(const std::string& edges) {
    uint32_t x = 0;
    uint32_t y = -1;

    auto current_edges = split(edges, ";");
    for (const auto& edge : current_edges) {
        if (x % NUMBER_OF_EDGES_IN_A_ROW == 0) {
            y++;
            x = 0;
        }
        uint32_t edge_player = std::stoi(edge);
        if (edge_player == NONE_PLAYER || m_edges_positions[y][x]) {
            x++;
            continue;
        }
        m_edges_positions[y][x] = true;
        add_new_edge(x, y, edge_player);
        x++;
    }
}

void GUIClient::add_new_edge(uint32_t x, uint32_t y, uint32_t player_number) {
    sf::Sprite edge;
    switch (static_cast<PlayerType>(player_number))
    {
    case PlayerType::BLUE:
        edge.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::EDGE_BLUE)]);
        break;
    case PlayerType::GREEN:
        edge.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::EDGE_GREEN)]);
        break;
    case PlayerType::RED:
        edge.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::EDGE_RED)]);
        break;
    case PlayerType::YELLOW:
        edge.setTexture(m_textures[static_cast<uint32_t>(TextureTypes::EDGE_YELLOW)]);
        break;
    default:
        break;
    }
    
    edge.setScale(0.6f, 0.6f);
    initialize_edge_position(edge, x, y);

    m_board_edges.push_back(edge);
}

void GUIClient::initialize_edge_position(sf::Sprite& edge, uint32_t x, uint32_t y) {
    
    auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
    auto size_of_resource_y = WINDOW_HIGHT * RESOURCE_HIGHT_IN_BACKGROUND / BACKGROUND_HIGHT;
    float shift_right = NUMBER_OF_EDGES_IN_A_COLUMN / 2;
    shift_right -= y;
    shift_right = std::max(shift_right, 0.0f);
    shift_right = x - shift_right;
    shift_right /= 2;

    float temp = NUMBER_OF_EDGES_IN_A_COLUMN / 2.0f - y;
    temp = std::abs(std::floor(temp)) / 4;
    shift_right += temp;

    auto x1 = (FIRST_POSITION_OF_EDGE_X * WINDOW_WIDTH / BACKGROUND_WIDTH) + size_of_resource_x * shift_right;
    auto y1 = (FIRST_POSITION_OF_EDGE_Y * WINDOW_HIGHT / BACKGROUND_HIGHT) + size_of_resource_y * (y / 2 * 0.757f);


    if (y % 2 == 1) {
        edge.setRotation(90);
    }
    else { 
        if (y == 0) {
            x--;
            x1 += size_of_resource_x / 2;
        }
        if (x % 2 == 1) {
            x1 += 14.0f;
            y1 -= 28.0f;
            edge.setRotation(153);
        }
        else {
            x1 += 13.0f;
            y1 -= 9.0f;
            edge.setRotation(-153);
        }
    }
    
    edge.setPosition(x1, y1);
}

void GUIClient::set_robber_position(const std::string& robber_location) {
    uint32_t x = stoi(split(robber_location, ",")[0]);
    uint32_t y = stoi(split(robber_location, ",")[1]);

    auto size_of_resource_x = WINDOW_WIDTH * RESOURCE_WIDTH_IN_BACKGROUND / BACKGROUND_WIDTH;
    auto size_of_resource_y = WINDOW_HIGHT * RESOURCE_HIGHT_IN_BACKGROUND / BACKGROUND_HIGHT;

    float first_position_in_board_x = FIRST_POSITION_OF_RESOURCE_X * WINDOW_WIDTH / BACKGROUND_WIDTH;
    float first_position_in_board_y = FIRST_POSITION_OF_RESOURCE_Y * WINDOW_HIGHT / BACKGROUND_HIGHT;

    auto x1 = first_position_in_board_x + (x - 1.0f + 0.5f * y) * size_of_resource_x;
    auto y1 = first_position_in_board_y + y * size_of_resource_y * 0.757f;

    m_robber.setPosition(x1 + 35.0f, y1 + 40.0f);
}

void GUIClient::update_dices(uint32_t first_dice, uint32_t second_dice) {
    m_dice_1.setTexture(m_textures[first_dice + static_cast<uint32_t>(TextureTypes::DICE_1) - 1]);
    m_dice_2.setTexture(m_textures[second_dice + static_cast<uint32_t>(TextureTypes::DICE_1) - 1]);
}

void GUIClient::update_available_resources(const std::unordered_map<ResourceType, uint32_t>& resources) {
    m_available_resources_texts[static_cast<uint32_t>(ResourceType::CLAY)].setString(std::to_string(resources.at(ResourceType::CLAY)));
    m_available_resources_texts[static_cast<uint32_t>(ResourceType::SHEEP)].setString(std::to_string(resources.at(ResourceType::SHEEP)));
    m_available_resources_texts[static_cast<uint32_t>(ResourceType::STONE)].setString(std::to_string(resources.at(ResourceType::STONE)));
    m_available_resources_texts[static_cast<uint32_t>(ResourceType::WHEAT)].setString(std::to_string(resources.at(ResourceType::WHEAT)));
    m_available_resources_texts[static_cast<uint32_t>(ResourceType::TREE)].setString(std::to_string(resources.at(ResourceType::TREE)));
}

void GUIClient::render_loop() {
    
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                m_window.close();
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                std::cout << "########### mouse : " << sf::Mouse::getPosition(m_window).x << "," << sf::Mouse::getPosition(m_window).y << std::endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
            {
                for (size_t i = 0; i < m_board_resources.size(); i++)
                {
                    m_background.setPosition(m_background.getPosition().x - 0.265f, m_background.getPosition().y);
                    m_board_resources[i].setPosition(m_board_resources[i].getPosition().x - 5.0f, m_board_resources[i].getPosition().y);
                    m_board_resources_numbers[i].setPosition(m_board_resources_numbers[i].getPosition().x - 5.0f, m_board_resources_numbers[i].getPosition().y);
                }
                
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                for (size_t i = 0; i < m_board_resources.size(); i++)
                {
                    m_background.setPosition(m_background.getPosition().x + 0.265f, m_background.getPosition().y);
                    m_board_resources[i].setPosition(m_board_resources[i].getPosition().x + 5.0f, m_board_resources[i].getPosition().y);
                    m_board_resources_numbers[i].setPosition(m_board_resources_numbers[i].getPosition().x + 5.0f, m_board_resources_numbers[i].getPosition().y);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                for (size_t i = 0; i < m_board_resources.size(); i++)
                {
                    m_background.setPosition(m_background.getPosition().x, m_background.getPosition().y - 0.265f);
                    m_board_resources[i].setPosition(m_board_resources[i].getPosition().x, m_board_resources[i].getPosition().y - 5.0f);
                    m_board_resources_numbers[i].setPosition(m_board_resources_numbers[i].getPosition().x, m_board_resources_numbers[i].getPosition().y - 5.0f);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                for (size_t i = 0; i < m_board_resources.size(); i++)
                {
                    m_background.setPosition(m_background.getPosition().x, m_background.getPosition().y + 0.265f);
                    m_board_resources[i].setPosition(m_board_resources[i].getPosition().x, m_board_resources[i].getPosition().y + 5.0f);
                    m_board_resources_numbers[i].setPosition(m_board_resources_numbers[i].getPosition().x, m_board_resources_numbers[i].getPosition().y + 5.0f);
                }
            }

        }
        draw();
    }
}

void GUIClient::draw() {
    m_window.clear(sf::Color::White);
    m_window.draw(m_background);
    for (size_t i = 0; i < m_board_resources.size(); i++) {
        m_window.draw(m_board_resources[i]);
    }
    for (size_t i = 0; i < m_board_resources_numbers.size(); i++) {
        m_window.draw(m_board_resources_numbers[i]);
    }
    for (size_t i = 0; i < m_board_structures.size(); i++)    {
        m_window.draw(m_board_structures[i]);
    }
    for (size_t i = 0; i < m_board_edges.size(); i++) {
        m_window.draw(m_board_edges[i]);
    }
    for (size_t i = 0; i < m_available_resources_images.size(); i++) {
        m_window.draw(m_available_resources_images[i]);
        m_window.draw(m_available_resources_texts[i]);
    }
    m_window.draw(m_robber);
    m_window.draw(m_dice_1);
    m_window.draw(m_dice_2);
    m_window.display();
}

GUIClient::~GUIClient(){
}
