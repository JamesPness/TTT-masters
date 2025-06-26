#include <iostream>
#include <fstream>
#include <vector>
#include <raylib.h>

class player_banner {
    private:
        std::string player_name;
        std::string creator_tag;
        std::string pic_path;
        Texture2D profile_pic;
        float width;
        float height;
        Vector2 position;
        
    public:
        player_banner();
        player_banner(std::string config_path);

        void load_profile_texture(float _height, float _width);
        void draw_right(Vector2 _position, float _width, float _height, bool turn);
        void draw_left(Vector2 _position, float _width, float _height, bool turn);

        std::string get_creator_tag(){
            return creator_tag;
        };

        void set_creator_tag(std::string creator){
            creator_tag = creator;
        };

        std::string get_pic_path(){
            return pic_path;
        };

        void set_pic_path(std::string path){
            pic_path = path;
        };

        std::string get_player_name(){
            return player_name;
        };

        void set_player_name(std::string name){ 
            player_name = name; 
        };
        float get_width(){
            return width;
        };
        float get_height(){
            return height;
        };

        Vector2 get_position(){
            return position;
        }
        //void set_player_pic()
};