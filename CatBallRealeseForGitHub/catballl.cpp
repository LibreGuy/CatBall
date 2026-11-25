              #include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Kitten Ball");
    InitAudioDevice();
    SetTargetFPS(60);

    // Zvukovi i teksture
    Texture2D catTexture    = LoadTexture("kit.png");
    Texture2D player2Sprite = LoadTexture("catTwo.png"); 
    Texture2D portalTexture = LoadTexture("portal.png");
    Texture2D appleTxt      = LoadTexture("apple.png");

    Sound MenuMusic   = LoadSound("bruh.wav");
    Sound coinCollect = LoadSound("coin.wav");
    Sound PortalTP    = LoadSound("portal.wav");
    Sound StartSound  = LoadSound("start.wav");
    Sound Eat         = LoadSound("Eat.mp3");
// touch dugmad
int buttonupx = 80;
int buttonupy = 310;

int buttondownx = 80;
int buttondowny = 380;

int buttonleftx = 20;
int buttonlefty = 380;

int buttonrightx = 140;
int buttonrighty = 380;

int shootbuttonx = 680;
int shootbuttony = 350;
    // Varijable za macu (Player 1)
    int xPosCat = 200;
    int yPosCat = 250;
    int SpeedOfCat = 3;

    // Varijable za Player 2
    int player2x = 400;
    int player2y = 250;
    int player2Speed = 3;
    bool TwoPlayer = false;

    // Varijable za lopticu
    int ballX = 400;
    int ballY = 225;
    int ballRadius = 15;

    // Varijable za portal i jabuku
    int portalx = 400;
    int portaly = 200;
    int applex = 200;
    int appley = 150;

    // Varijable za stanje igre i meni
    bool Musik = false;
    int score = 0;
    int p2score = 0;
    int level = 1;  
    int gameState = 2;

    int buttonRadiusX = 400;
    int buttonRadiusY = 50;
    int buttonx = 200;
    int buttony = 225;
    int button2playerx = 200;
    int button2playery = 150;

    // Varijable za neprijatelja (Plava kocka)
    int enemyx = 400;
    int enemyY = 50;
    int enemySize = 50; // Sirina i visina kocke
    int enemyspeed = 5;

    while (!WindowShouldClose())    
    { 
        // Kontrola muzike
        if (IsKeyDown(KEY_M)) Musik = true;
        if (IsKeyDown(KEY_N)) Musik = false;

        if (!IsSoundPlaying(MenuMusic) && Musik) PlaySound(MenuMusic);
        if (IsSoundPlaying(MenuMusic) && !Musik) StopSound(MenuMusic);

        // --- STANJE 2: GLAVNI MENI ---
        if (gameState == 2)
        { 
            Vector2 mousePos = GetMousePosition();
            Rectangle Bbox   = { (float)buttonx, (float)buttony, (float)buttonRadiusX, (float)buttonRadiusY };
            Rectangle Bbox2p = { (float)button2playerx, (float)button2playery, (float)buttonRadiusX, (float)buttonRadiusY };

            if (CheckCollisionPointRec(mousePos, Bbox) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                gameState = 0;
                TwoPlayer = false;
                PlaySound(StartSound);
            }
            if (CheckCollisionPointRec(mousePos, Bbox2p) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                gameState = 0;
                TwoPlayer = true;
                PlaySound(StartSound);
            }

            BeginDrawing();
                ClearBackground(RED);
                DrawText("Kitten Ball!", 270, 50, 50, WHITE);
                
                DrawRectangleRec(Bbox2p, BLUE);
                DrawText("TWO Player", 330, button2playery + 13, 24, WHITE);
                
                DrawRectangleRec(Bbox, DARKGREEN);
                DrawText("START", 355, buttony + 13, 24, WHITE);
            EndDrawing();
        }

        // --- STANJE 0: GAMEPLAY ---
        if (gameState == 0)
        {
            // Kretanje neprijatelja
            enemyx += enemyspeed;
            if (enemyx >= screenWidth - enemySize) enemyx = 0;
            
            // Kretanje Player 1
            if (IsKeyDown(KEY_LEFT))  xPosCat -= SpeedOfCat;
            if (IsKeyDown(KEY_RIGHT)) xPosCat += SpeedOfCat;
            if (IsKeyDown(KEY_UP))    yPosCat -= SpeedOfCat;
            if (IsKeyDown(KEY_DOWN))  yPosCat += SpeedOfCat;

            // Granice ekrana za Player 1
            if (xPosCat < 0) xPosCat = 0;
            if (xPosCat > screenWidth - catTexture.width) xPosCat = screenWidth - catTexture.width;
            if (yPosCat < 0) yPosCat = 0;
            if (yPosCat > screenHeight - catTexture.height) yPosCat = screenHeight - catTexture.height;

            // Kretanje Player 2
            if (TwoPlayer)
            {
                if (IsKeyDown(KEY_W)) player2y -= player2Speed;
                if (IsKeyDown(KEY_S)) player2y += player2Speed;
                if (IsKeyDown(KEY_A)) player2x -= player2Speed;
                if (IsKeyDown(KEY_D)) player2x += player2Speed;

                if (player2x < 0) player2x = 0;
                if (player2x > screenWidth - player2Sprite.width) player2x = screenWidth - player2Sprite.width;
                if (player2y < 0) player2y = 0;
                if (player2y > screenHeight - player2Sprite.height) player2y = screenHeight - player2Sprite.height;
            }

            // Definišemo SVE hitboxove PRIJE provjere sudara
            Rectangle catBox    = { (float)xPosCat, (float)yPosCat, (float)catTexture.width, (float)catTexture.height };
            Rectangle p2box     = { (float)player2x, (float)player2y, (float)player2Sprite.width, (float)player2Sprite.height };
            Rectangle ballBox   = { (float)ballX - ballRadius, (float)ballY - ballRadius, (float)ballRadius * 2, (float)ballRadius * 2 };
            Rectangle portalBox = { (float)portalx, (float)portaly, (float)portalTexture.width, (float)portalTexture.height };  
            Rectangle enemyBox  = { (float)enemyx, (float)enemyY, (float)enemySize, (float)enemySize };
            Rectangle appleBox  = { (float)applex, (float)appley, (float)appleTxt.width, (float)appleTxt.height };

            // Skupljanje loptice (Player 1 i Player 2)
            if (CheckCollisionRecs(catBox, ballBox))
            {
                score += 1;
                ballX = GetRandomValue(50, screenWidth - 50);
                ballY = GetRandomValue(50, screenHeight - 50);
                PlaySound(coinCollect);
            }
            else if (TwoPlayer && CheckCollisionRecs(p2box, ballBox))
            {
                p2score += 1;
                ballX = GetRandomValue(50, screenWidth - 50);
                ballY = GetRandomValue(50, screenHeight - 50);
                PlaySound(coinCollect);
            }

            // Prelazak na Level 2
            if ((score >= 5 || p2score >= 5) && level == 1)
            {
                level = 2;
                enemyspeed = 7;
            }

            // Mehanike za Level 2+ (Portal i Jabuka)
            if (level >= 2)
            {
                if (CheckCollisionRecs(catBox, portalBox))
                {
                    xPosCat = GetRandomValue(50, screenWidth - 50);
                    yPosCat = ballY;
                    PlaySound(PortalTP);
                }
                if (TwoPlayer && CheckCollisionRecs(p2box, portalBox))
                {
                    player2x = GetRandomValue(50, screenWidth - 50);
                    player2y = ballY;
                    PlaySound(PortalTP);
                }

                // Skupljanje jabuke (Player 1)
                if (CheckCollisionRecs(catBox, appleBox))
                {
                    score += 2;
                    SpeedOfCat += 2;
                    if (SpeedOfCat > 10) SpeedOfCat = 10;
                    applex = GetRandomValue(50, screenWidth - 50);
                    appley = GetRandomValue(50, screenHeight - 50);
                    PlaySound(Eat);
                }

                // Skupljanje jabuke (Player 2)
                if (TwoPlayer && CheckCollisionRecs(p2box, appleBox))
                {
                    p2score += 2;
                    player2Speed += 2;
                    if (player2Speed > 10) player2Speed = 10;
                    applex = GetRandomValue(50, screenWidth - 50);
                    appley = GetRandomValue(50, screenHeight - 50);
                    PlaySound(Eat);
                }
            }

            // Sudar sa neprijateljem (Plava kocka - Rectangle hitbox)
            if (CheckCollisionRecs(catBox, enemyBox))
            {
                xPosCat = 200;
                yPosCat = 250; score = 0;
                
            }

            if (TwoPlayer && CheckCollisionRecs(p2box, enemyBox))
            {
                player2x = 400;
                player2y = 250;
                if (p2score > 0) score = 0;
            }

            // --- CRTANJE GAMEPLAY-A ---
            BeginDrawing();
                ClearBackground(RED); // Crvena pozadina na svim levelima

                // Crtanje objekata za Level 2+
                if (level >= 2)
                {
                    DrawTexture(portalTexture, portalx, portaly, WHITE);
                    DrawTexture(appleTxt, applex, appley, WHITE);
                }

                // Crtanje igraca, loptice i plave kocke (neprijatelja)
                DrawTexture(catTexture, xPosCat, yPosCat, WHITE);
                if (TwoPlayer) DrawTexture(player2Sprite, player2x, player2y, WHITE);
                
                DrawCircle(ballX, ballY, (float)ballRadius, YELLOW); // Zuta loptica da se bolje vidi na crvenoj pozadini
                DrawRectangleRec(enemyBox, BLUE); // Plava kocka

                // UI (Rezultat i Level) - bijeli tekst da bude citljiv na crvenom
                DrawText(TextFormat("P1 Score: %i", score), 10, 10, 20, WHITE);
                if (TwoPlayer) DrawText(TextFormat("P2 Score: %i", p2score), 10, 35, 20, SKYBLUE);
                DrawText(TextFormat("Level: %i", level), screenWidth - 100, 10, 20, YELLOW);

            EndDrawing();
        }
    }

    // Oslobadjanje resursa
    UnloadTexture(catTexture);
    UnloadTexture(player2Sprite);
    UnloadTexture(portalTexture);
    UnloadTexture(appleTxt);

    UnloadSound(MenuMusic);
    UnloadSound(coinCollect);
    UnloadSound(PortalTP);
    UnloadSound(StartSound);
    UnloadSound(Eat);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
