#include <raylib.h>

class Button{
    private:
        Texture2D texture;
        Vector2 position;
    public:

        Button(const char* imagePath,Vector2 imagePosition,float scale){
            Image image = LoadImage(imagePath);
            int originalWidht = image.width;
            int originalHeight = image.height;
            int newWidht = static_cast<int> (originalWidht * scale);
            int newHeight = static_cast<int> (originalHeight * scale);
            ImageResize(&image,newWidht,newHeight);
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = imagePosition;
        }

        ~Button(){
            UnloadTexture(texture);
        }

        void Draw(){
            DrawTexture(texture, position.x, position.y,WHITE);
        }

        bool isPressed(Vector2 mousePos,bool mousePressed){
            Rectangle rect = {position.x,position.y,(float)texture.width,(float)texture.height};
            if(CheckCollisionPointRec(mousePos,rect) && mousePressed){
                return true;
            }
            return false;
        }
};