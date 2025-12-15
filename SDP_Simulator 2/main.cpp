#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHRandom.h"
#include "FEHImages.h"
#include "FEHKeyboard.h"
#include <cstring>
#include <stdio.h>
#define main_menu_state 0
#define mode_select_state 1
#define blackjack_state 2
#define character_select_state 3
#define world_state 4
#define shop_screen_state 5
#define bank_state 6
#define inventory_state 7
#define credits_state 8
#define statistics_state 9
#define instructions_state 10
#define quit 11

#define playerwin 1 
#define playerloss -1
#define playertie 0

#define max_cards 11
#define max_inventory 10
#define win_reward 100
#define loss_penalty 50
#define starting_money 100

#define max_session_time 300
#define max_round_time 60

/*
 Hand Class
 
 This class manages a player's or dealer's hand of cards in blackjack.
 It stores the cards, keeps track of how many cards are in the hand, and calculates the total hand value.
 
 Private Members:
   - cards[max_cards]: Array that stores card values (2-10)
   - count: Number of cards currently in the hand
 
 Public Members:
   - AddCard(int card): Adds a new card to the hand
   - GetValue(): Returns the total value of all cards in the hand
   - GetCount(): Returns how many cards are in the hand
   - GetCard(int index): Returns the card at a specific index
   - Clear(): Resets the hand to empty for a new round
 
 Author: Akshit Jalli
 */
class Hand {
private:
    int cards[max_cards];
    int count;
    
public:
    // constructor initializes empty hand
    Hand() {
        count = 0;
        int i = 0;
        while(i < max_cards) {
            cards[i] = 0;
            i = i + 1;
        }
    }
    
    // adds a card to the hand
    void AddCard(int card) {
        if(count < max_cards) {
            cards[count] = card;
            count = count + 1;
        }
    }
    
    // calculates and returns total hand value
    int GetValue() {
        int value = 0;
        int i = 0;
        while(i < count) {
            value = value + cards[i];
            i = i + 1;
        }
        return value;
    }
    
    // returns number of cards in hand
    int GetCount() {
        return count;
    }
    
    // returns card at specified index
    int GetCard(int index) {
        if(index >= 0 && index < count) {
            return cards[index];
        }
        return 0;
    }
    
    // clears the hand for new round
    void Clear() {
        count = 0;
        int i = 0;
        while(i < max_cards) {
            cards[i] = 0;
            i = i + 1;
        }
    }
};

/*
 Player Class
 
 This class represents a human player in the blackjack game.
 It manages the player's hand of cards, their money, and whether they've chosen to stand.
 
 Private Members:
   - hand: Hand object that stores the player's cards
   - money: The player's current money amount
   - hasStood: Whether the player has chosen to stand (stop taking cards)
 
 Public Members:
   - GetHand(): Returns a reference to the player's hand
   - GetMoney(): Returns the player's current money
   - AddMoney(int amount): Adds money to the player's total
   - Stand(): Player chooses to stand and end their turn
   - HasStood(): Returns true if the player has stood, false otherwise
   - Reset(): Resets the player for a new round
 
 Author: Akshit Jalli
 */
class Player {
private:
    Hand hand;
    int money;
    bool hasStood;
    
public:
    // constructor initializes player with starting money
    Player(int startingMoney) {
        money = startingMoney;
        hasStood = false;
    }
    
    // returns pointer to player's hand
    Hand* GetHand() {
        return &hand;
    }
    
    // returns player's current money
    int GetMoney() {
        return money;
    }
    
    // sets player's money amount
    void SetMoney(int amount) {
        money = amount;
        if(money < 0) {
            money = 0;
        }
    }
    
    // adds money to player's total
    void AddMoney(int amount) {
        money = money + amount;
        if(money < 0) {
            money = 0;
        }
    }
    
    // player chooses to stand
    void Stand() {
        hasStood = true;
    }
    
    // checks if player has stood
    bool HasStood() {
        return hasStood;
    }
    
    // resets player for new round
    void Reset() {
        hand.Clear();
        hasStood = false;
    }
};

/*
 Dealer Class
 
 This class represents the dealer (AI opponent) in single-player blackjack.
 It manages the dealer's hand and implements the dealer AI logic for playing.
 
 Private Members:
   - hand: Hand object that stores the dealer's cards
   - hasStood: Whether the dealer has finished playing their turn
 
 Public Members:
   - GetHand(): Returns a reference to the dealer's hand
   - Stand(): Dealer chooses to stand and end their turn
   - HasStood(): Returns true if the dealer has stood, false otherwise
   - ShouldHit(): Returns true if dealer should hit (follows Rule of 17: hit if 16 or less)
   - Reset(): Resets the dealer for a new round
 
 Author: Kerem Cakmak
 */
class Dealer {
private:
    Hand hand;
    bool hasStood;
    
public:
    // constructor initializes dealer
    Dealer() {
        hasStood = false;
    }
    
    // returns pointer to dealer's hand
    Hand* GetHand() {
        return &hand;
    }
    
    // dealer chooses to stand
    void Stand() {
        hasStood = true;
    }
    
    // checks if dealer has stood
    bool HasStood() {
        return hasStood;
    }
    
    // dealer AI follows rule of 17
    bool ShouldHit() {
        int value = hand.GetValue();
        if(value <= 16) {
            return true;
        }
        return false;
    }
    
    // resets dealer for new round
    void Reset() {
        hand.Clear();
        hasStood = false;
    }
};

/*
 Button Class
 
 This class represents a clickable button on the screen.
 It stores the position, size, and text for UI elements like menu buttons.
 
 Private Members:
   - x, y: Button position (top-left corner coordinates)
   - w, h: Button width and height in pixels
   - text: Button label text that gets displayed
 
 Public Members:
   - SetPosition(int x, int y): Sets the button's position on screen
   - SetSize(int w, int h): Sets the button's width and height
   - SetText(const char* text): Sets the text that appears on the button
   - GetX(), GetY(), GetW(), GetH(): Get the button's position and size values
   - GetText(): Get the button's text label
 
 Author: Akshit Jalli
 */
class Button {
private:
    int x;
    int y;
    int w;
    int h;
    const char *text;
    
public:
    // constructor initializes button
    Button() {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
        text = "";
    }
    
    // sets button position
    void SetPosition(int xPos, int yPos) {
        x = xPos;
        y = yPos;
    }
    
    // sets button size
    void SetSize(int width, int height) {
        w = width;
        h = height;
    }
    
    // sets button text
    void SetText(const char* buttonText) {
        text = buttonText;
    }
    
    // getter methods for position and size
    int GetX() { return x; }
    int GetY() { return y; }
    int GetW() { return w; }
    int GetH() { return h; }
    
    // returns button text
    const char* GetText() { return text; }
};

struct shop_item {
    int id;
    const char *name;
    int cost;
};

int inventory_items;
int player_money;
bool game_on;
shop_item inventory[max_inventory];

int total_wins;
int total_losses;

const char* loss_quotes[10] = {
    "The house always wins.",
    "Gambling is a tax.",
    "The longer you play, the more you lose.",
    "You are never due for a win.",
    "Gambling steals from your future.",
    "Fold your money and keep it.",
    "Chasing losses leads to ruin.",
    "The only winning move is not playing.",
    "Bet on yourself, not the odds.",
    "Your peace of mind is priceless."
};

double session_start_time;
double current_round_start_time;

FEHImage main_menu_background;
int main_menu_image_loaded = 0;

FEHImage game_background;
int game_background_loaded = 0;

FEHImage card_images[11];
int card_images_loaded = 0;

FEHImage world_background;
int world_background_loaded = 0;

FEHImage boy_sprite;
FEHImage girl_sprite;
int character_sprites_loaded = 0;

FEHImage shop_item_images[10];
FEHImage shop_background;
FEHImage arrow_left;
FEHImage arrow_right;
int shop_images_loaded = 0;

int selected_character = 0;
int player_x = 160;
int player_y = 120;

shop_item shop_items[] = {
    {0, "Donate", 500},
    {1, "Dog", 500},
    {2, "Lab Report", 1000},
    {3, "FEH Textbook", 600},
    {4, "Car Donation", 10000},
    {5, "Library Card", 100},
    {6, "Plant", 150},
    {7, "Teaching Manual", 1000},
    {8, "Present", 3000}
};

const char* item_descriptions[] = {
    "Help those in need",
    "Loyal companion",
    "Carter's lab work",
    "Study guide",
    "Luxury vehicle",
    "Access to books",
    "Green decoration",
    "Professor's notes",
    "Holiday gift"
};  


/*
 Blackjackbutton Function
 
 This function draws a button with white fill, red borders, and red text.
 It uses a triple border style for an arcade aesthetic look.
 
 Input Arguments:
   - a: Button object to draw on the screen
 
 Return Value: None (void)
 
 Author: Akshit Jalli
 */
void Blackjackbutton(Button a){
    int buttonfillcolor = WHITE;
    int buttontextcolor = RED;

    LCD.SetFontColor(buttonfillcolor);
    LCD.FillRectangle(a.GetX(), a.GetY(), a.GetW(), a.GetH());
    LCD.SetFontColor(buttontextcolor);
    LCD.DrawRectangle(a.GetX(), a.GetY(), a.GetW(), a.GetH());
    LCD.DrawRectangle(a.GetX() + 1, a.GetY() + 1, a.GetW() - 2, a.GetH() - 2);
    LCD.DrawRectangle(a.GetX() + 2, a.GetY() + 2, a.GetW() - 4, a.GetH() - 4);

    LCD.SetFontScale(0.6);
    int textlength = strlen(a.GetText());
    int textx = a.GetX() + (a.GetW() / 2) - (3 * textlength);
    int texty = a.GetY() + (a.GetH() / 2) - 5;
    LCD.SetFontColor(buttontextcolor);
    LCD.WriteAt(a.GetText(), textx, texty);
    LCD.SetFontScale(1.0);
 }

void blackjackbutton2(Button d){
    int fillcolor = GREEN;
    int textcolor = RED;
    LCD.SetFontColor(fillcolor);
    LCD.FillRectangle(d.GetX(), d.GetY(), d.GetW(), d.GetH());
    LCD.SetFontColor(textcolor);
    LCD.DrawRectangle(d.GetX(), d.GetY(), d.GetW(), d.GetH());
    LCD.DrawRectangle(d.GetX() + 1, d.GetY() + 1, d.GetW() - 2, d.GetH() - 2);
    LCD.DrawRectangle(d.GetX() + 2, d.GetY() + 2, d.GetW() - 4, d.GetH() - 4);
    
    LCD.SetFontScale(0.6);
    int textlength = strlen(d.GetText());
    int textx = d.GetX() + (d.GetW() / 2) - (3 * textlength);
    int texty = d.GetY() + (d.GetH() / 2) - 5;
    LCD.SetFontColor(textcolor);
    LCD.WriteAt(d.GetText(), textx, texty);
    LCD.SetFontScale(1.0);
}

/*
 animation Function
 
 This function handles touch input detection for button arrays.
 It returns the index of the pressed button, or -1 if no button was pressed.
 It also provides visual feedback by flashing the button when presed.
 
 Input Arguments:
   - buttons[]: Array of Button objects to check for touches
   - numbuttons: Number of buttons in the array
 
 Return Value: Index of the pressed button (0 to numbuttons-1), or -1 if none was pressed
 
 Author: Akshit Jalli
 */
int animation(Button buttons[], int numbuttons){
float touch_x;
float touch_y;

if(LCD.Touch(&touch_x, &touch_y)){
    while(LCD.Touch(&touch_x, &touch_y)){
        LCD.Update();
    }
    int i = 0;
    while(i < numbuttons){
        if(touch_x >= buttons[i].GetX() && touch_x <= buttons[i].GetX() + buttons[i].GetW() && touch_y >= buttons[i].GetY() && touch_y <= buttons[i].GetY() + buttons[i].GetH()){
            blackjackbutton2(buttons[i]);
            LCD.Update();
            Sleep(0.2);
            return i;
        }
        i = i + 1;
    }
}
return -1;
}

/*
 LoadMainMenuImage Function
 
 This function loads the main menu background image from file.
 It uses a flag to prevent reloading if the image is already loaded.
 
 Input Arguments: None
 
 Return Value: None (void)
 
 Author: Kerem Cakmak
 */
void LoadMainMenuImage() {
    if(main_menu_image_loaded == 1) {
        return;
    }
    
    main_menu_background.Open("mainmenufinal.png");
    main_menu_image_loaded = 1;
}

/*
 LoadGameImages Function
 
 This function loads the game background and all card images from files.
 It uses a flag to prevent reloading if images are already loaded.
 
 Input Arguments: None
 
 Return Value: None (void)
 
 Author: Kerem Cakmak
 */
void LoadGameImages() {
    if(game_background_loaded == 1) {
        return;
    }
    
    // load game background image
    game_background.Open("dealer3.png");
    game_background_loaded = 1;
    
    // load card images for values 2 through 10
    card_images[2].Open("twocard.png");
    card_images[3].Open("threecard.png");
    card_images[4].Open("fourcard.png");
    card_images[5].Open("fivecard.png");
    card_images[6].Open("sixcard.png");
    card_images[7].Open("sevencard.png");
    card_images[8].Open("eightcard.png");
    card_images[9].Open("ninecard.png");
    card_images[10].Open("tencard.png");
    
    card_images_loaded = 1;
}

/*
 LoadWorldImages Function
 
 This function loads the world background, character sprites, and shop item images.
 It uses flags to prevent reloading if images are alredy loaded.
 
 Input Arguments: None
 
 Return Value: None (void)
 
 Author: Akshit Jalli, Kerem Cakmak
 */
void LoadWorldImages() {
    if(world_background_loaded == 1) {
        return;
    }
    
    // load world background image
    world_background.Open("world.png");
    world_background_loaded = 1;
    
    // load character sprite images
    boy_sprite.Open("Boy.png");
    girl_sprite.Open("Girl.png");
    character_sprites_loaded = 1;
    
    // load shop item images
    shop_item_images[0].Open("DONATE.png");
    shop_item_images[1].Open("dog.png");
    shop_item_images[2].Open("LAB REPORT.png");
    shop_item_images[3].Open("FEH_TEXTBOOK.png");
    shop_item_images[4].Open("lambo.png");
    shop_item_images[5].Open("LIBRARY CARD.png");
    shop_item_images[6].Open("plant2.png");
    shop_item_images[7].Open("TEACHING_MANUAL.png");
    shop_item_images[8].Open("present.png");
    
    // load shop background image
    shop_background.Open("shopfinal.png");
    
    // load navigation arrow images
    arrow_left.Open("ArrowLeft.png");
    arrow_right.Open("ArrowRight.png");
    
    shop_images_loaded = 1;
}

/*
 DealCard Function
 
 This function generates a random card value between 2 and 10.
 It uses FEHRandom to meet the random generation requirement for the project.
 
 Input Arguments: None
 
 Return Value: Integer card value (2-10, no aces or face cards)
 
 Author: Akshit Jalli, Kerem Cakmak
 Reference: FEHRandom library documentation
 */
int DealCard() {
    FEHRandom Random;
    int randomNum = Random.RandInt();
    // returns random card value between 2 and 10
    int card = (randomNum % 9) + 2;
    return card;
}


int DetermineWinner(int playerValue, int dealerValue) {
    // player busts if over 21
    if(playerValue > 21) {
        return playerloss;
    }
    // dealer busts if over 21
    if(dealerValue > 21) {
        return playerwin;
    }
    // compare values if both valid
    if(playerValue > dealerValue) {
        return playerwin;
    }
    if(dealerValue > playerValue) {
        return playerloss;
    }
    // equal values result in tie
    return playertie;
}

/*
 DrawCardPlaceholder Function
 
 This function draws a visual representation of a card on the screen.
 It uses loaded card images if available, otherwise draws a placeholder rectangle.
 
 Input Arguments:
   - rank: Card value (2-10)
   - x: X coordinate for card position
   - y: Y coordinate for card position
 
 Return Value: None (void)
 
 Author: Akshit Jalli, Kerem Cakmak
 */
void DrawCardPlaceholder(int rank, int x, int y) {
    // use card image if available
    if(card_images_loaded == 1 && rank >= 2 && rank <= 10) {
        card_images[rank].Draw(x, y);
    } else {
        // draw placeholder rectangle if image not loaded
        LCD.SetFontColor(WHITE);
        LCD.FillRectangle(x, y, 10, 20);
        LCD.SetFontColor(BLACK);
        LCD.DrawRectangle(x, y, 10, 20);
        
        char rankStr[3];
        sprintf(rankStr, "%d", rank);
        LCD.SetFontColor(BLACK);
        LCD.WriteAt(rankStr, x + 2, y + 6);
    }
}

/*
 DrawCardBack Function
 
 This function draws a card back (hidden card) on the screen.
 It's used to hide the dealer's second card during the player's turn.
 
 Input Arguments:
   - x: X coordinate for card position
   - y: Y coordinate for card position
 
 Return Value: None (void)
 
 Author: Akshit Jalli, Kerem Cakmak
 */
void DrawCardBack(int x, int y) {
    LCD.SetFontColor(RED);
    LCD.FillRectangle(x, y, 10, 20);
    LCD.SetFontColor(BLACK);
    LCD.DrawRectangle(x, y, 10, 20);
}

/*
 CheckTimeLimit Function
 
 This function checks if session or round time limits have been exceeded.
 It implements gambling prevention by limiting play time to meet VGV requirements.
 
 Input Arguments: None
 
 Return Value:
   - 0: No time limit exceeded
   - 1: Session time limit exceeded (5 minutes)
   - 2: Round time limit exceeded (60 seconds)
 
 Author: Akshit Jalli, Kerem Cakmak
 Reference: FEHUtility::TimeNow() documentation
 */
int CheckTimeLimit() {
    double current_time;
    current_time = TimeNow();
    
    // check if session time limit exceeded
    if(current_time - session_start_time > max_session_time) {
        return 1;
    }
    
    // check if round time limit exceeded
    if(current_time - current_round_start_time > max_round_time) {
        return 2;
    }
    
    return 0;
}

/*
 MainMenu Function
 
 This function displays the main menu screen with navigation buttons.
 It handles user input to navigate to different game states.
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - mode_select_state: If PLAY button pressed
   - character_select_state: If WORLD button pressed
   - inventory_state: If ITEMS button pressed
   - statistics_state: If Statistics button pressed
   - instructions_state: If Instructions button pressed
   - credits_state: If Credits button pressed
   - quit: If Quit button pressed
 
 Author: Akshit Jalli, Kerem Cakmak
 */
int MainMenu(){
    LCD.Clear(BLACK);
    
    // draw background image if loaded
    if(main_menu_image_loaded == 1) {
        main_menu_background.Draw(0, 0);
    }
    
    // draw title
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("BLACKJACK", 110, 10);

    // create menu buttons
    Button buttons[7];
    buttons[0].SetPosition(20, 30);
    buttons[0].SetSize(80, 30);
    buttons[0].SetText("PLAY");
    buttons[1].SetPosition(20, 65);
    buttons[1].SetSize(80, 30);
    buttons[1].SetText("WORLD");
    buttons[2].SetPosition(20, 100);
    buttons[2].SetSize(80, 30);
    buttons[2].SetText("ITEMS");
    buttons[3].SetPosition(20, 135);
    buttons[3].SetSize(100, 30);
    buttons[3].SetText("Statistics");
    buttons[4].SetPosition(20, 170);
    buttons[4].SetSize(100, 30);
    buttons[4].SetText("Instructions");
    buttons[5].SetPosition(20, 205);
    buttons[5].SetSize(80, 25);
    buttons[5].SetText("Credits");
    buttons[6].SetPosition(230, 10);
    buttons[6].SetSize(80, 25);
    buttons[6].SetText("Quit");
    
    int i = 0;
    while(i < 7) {
        Blackjackbutton(buttons[i]);
        i = i + 1;
    }
    
    LCD.Update();
    
    while(1) {
        int pressed = animation(buttons, 7);
        if(pressed == 0) {
            return mode_select_state;
        } else {
            if(pressed == 1) {
                return character_select_state;
            } else {
                if(pressed == 2) {
                    return inventory_state;
                } else {
                    if(pressed == 3) {
                        return statistics_state;
                    } else {
                        if(pressed == 4) {
                            return instructions_state;
                        } else {
                            if(pressed == 5) {
                                return credits_state;
                            } else {
                                if(pressed == 6) {
                                    return quit;
                                }
                            }
                        }
                    }
                }
            }
        }
        LCD.Update();
    }
}

/*
 ModeSelect Function
 
 This function displays the stats screen with win/loss tracking.
 It allows the user to start a game or return to main menu.
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - blackjack_state: If Play button pressed
   - main_menu_state: If Back button pressed
 
 Author: Akshit Jalli, Kerem Cakmak
 */
int ModeSelect() {
    LCD.Clear(BLACK);
    
    // draw bottom taskbar
    int taskbarY = 200;
    int taskbarHeight = 40;
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(0, taskbarY, 319, taskbarHeight);
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(0, taskbarY, 319, taskbarHeight);
    LCD.DrawRectangle(1, taskbarY + 1, 317, taskbarHeight - 2);
    
    // display title messages
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Play Responsibly", 50, 30);
    LCD.WriteAt("Stats Displayed Below", 50, 50);
    
    // display wins in taskbar
    LCD.SetFontColor(GREEN);
    LCD.SetFontScale(0.6);
    char winStr[15];
    sprintf(winStr, "Wins: %d", total_wins);
    LCD.WriteAt(winStr, 10, taskbarY + 12);
    
    // display losses in taskbar
    LCD.SetFontColor(RED);
    LCD.SetFontScale(0.6);
    char lossStr[15];
    sprintf(lossStr, "Loss: %d", total_losses);
    LCD.WriteAt(lossStr, 150, taskbarY + 12);
    LCD.SetFontScale(1.0);
    
    Button buttons[2];
    buttons[0].SetPosition(90, 100);
    buttons[0].SetSize(140, 40);
    buttons[0].SetText("Play");
    
    buttons[1].SetPosition(90, 160);
    buttons[1].SetSize(140, 35);
    buttons[1].SetText("Back");
    
    int i = 0;
    while(i < 2) {
        Blackjackbutton(buttons[i]);
        i = i + 1;
    }
    
    LCD.Update();
    
    while(1) {
        int pressed = animation(buttons, 2);
        if(pressed == 0) {
            return blackjack_state;
        } else {
            if(pressed == 1) {
                return main_menu_state;
            }
        }
        LCD.Update();
    }
}

/*
 PlayBlackjack Function
 
 This function implements the single-player blackjack game against dealer AI.
 It manages the game loop, player and dealer turns, and win/loss determination.
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - main_menu_state: Returns to main menu after game ends
 
 Author: Akshit Jalli
 */
int PlayBlackjack() {
    // initialize game objects
    Player player(player_money);
    Dealer dealer;
    int gameOver = 0;
    
    // start round timer
    current_round_start_time = TimeNow();
    
    // deal initial cards to player and dealer
    player.GetHand()->AddCard(DealCard());
    player.GetHand()->AddCard(DealCard());
    
    dealer.GetHand()->AddCard(DealCard());
    dealer.GetHand()->AddCard(DealCard());
    
    // main game loop
    while(gameOver == 0) {
        // check time limits
        int timeCheck = CheckTimeLimit();
        if(timeCheck > 0) {
            LCD.Clear(BLACK);
            LCD.SetFontColor(WHITE);
            if(timeCheck == 1) {
                LCD.WriteAt("Session Time Up!", 40, 100);
                LCD.WriteAt("Take a break!", 50, 130);
            } else {
                LCD.WriteAt("Round Time Up!", 50, 100);
            }
            LCD.WriteAt("Touch to continue", 30, 160);
            LCD.Update();
            
            float touch_x, touch_y;
            while(!LCD.Touch(&touch_x, &touch_y)) {
                LCD.Update();
            }
            while(LCD.Touch(&touch_x, &touch_y)) {
                LCD.Update();
            }
            return main_menu_state;
        }
        
        LCD.Clear(BLACK);
        
        // draw game background
        if(game_background_loaded == 1) {
            game_background.Draw(0, 0);
        }
        
        // display dealer cards
        LCD.SetFontColor(WHITE);
        LCD.SetFontScale(0.5);
        LCD.WriteAt("Dealer:", 10, 40);
        LCD.SetFontScale(1.0);
        if(player.HasStood() == false) {
            DrawCardPlaceholder(dealer.GetHand()->GetCard(0), 10, 60);
            DrawCardBack(25, 60);
        } else {
            int i = 0;
            int dealerCount = dealer.GetHand()->GetCount();
            while(i < dealerCount) {
                DrawCardPlaceholder(dealer.GetHand()->GetCard(i), 10 + (i * 15), 60);
                i = i + 1;
            }
        }
        
        // display player cards
        LCD.SetFontColor(WHITE);
        LCD.SetFontScale(0.5);
        LCD.WriteAt("Player:", 10, 120);
        LCD.SetFontScale(1.0);
        int i = 0;
        int playerCount = player.GetHand()->GetCount();
        while(i < playerCount) {
            DrawCardPlaceholder(player.GetHand()->GetCard(i), 10 + (i * 15), 140);
            i = i + 1;
        }
        int playerValue = player.GetHand()->GetValue();
        
        // draw bottom taskbar
        int taskbarY = 200;
        int taskbarHeight = 40;
        LCD.SetFontColor(BLACK);
        LCD.FillRectangle(0, taskbarY, 319, taskbarHeight);
        LCD.SetFontColor(WHITE);
        LCD.DrawRectangle(0, taskbarY, 319, taskbarHeight);
        LCD.DrawRectangle(1, taskbarY + 1, 317, taskbarHeight - 2);
        
        // display money in taskbar
        LCD.SetFontColor(YELLOW);
        LCD.SetFontScale(0.6);
        char moneyStr[20];
        sprintf(moneyStr, "$%d", player.GetMoney());
        LCD.WriteAt(moneyStr, 10, taskbarY + 12);
        
        // display player hand value in taskbar
        LCD.SetFontColor(WHITE);
        LCD.SetFontScale(0.6);
        char playerValStr[20];
        sprintf(playerValStr, "Player: %d", playerValue);
        LCD.WriteAt(playerValStr, 120, taskbarY + 12);
        
        // display dealer value only after player stands
        if(player.HasStood() == true) {
            int dealerValue = dealer.GetHand()->GetValue();
            LCD.SetFontColor(WHITE);
            LCD.SetFontScale(0.6);
            char dealerValStr[20];
            sprintf(dealerValStr, "Dealer: %d", dealerValue);
            LCD.WriteAt(dealerValStr, 220, taskbarY + 12);
        }
        
        LCD.SetFontScale(1.0);
        
        // player turn
        if(player.HasStood() == false) {
            // check if player busts
            if(playerValue > 21) {
                gameOver = 1;
            } else {
                // create hit and stand buttons
                Button actionButtons[2];
                actionButtons[0].SetPosition(20, 165);
                actionButtons[0].SetSize(80, 30);
                actionButtons[0].SetText("Hit");
                
                actionButtons[1].SetPosition(120, 165);
                actionButtons[1].SetSize(80, 30);
                actionButtons[1].SetText("Stand");
                
                i = 0;
                while(i < 2) {
                    Blackjackbutton(actionButtons[i]);
                    i = i + 1;
                }
                
                LCD.Update();
                
                // handle player action
                int action = animation(actionButtons, 2);
                if(action == 0) {
                    // player hits
                    player.GetHand()->AddCard(DealCard());
                } else {
                    if(action == 1) {
                        // player stands
                        player.Stand();
                    }
                }
            }
        } else {
            // dealer turn
            if(dealer.HasStood() == false) {
                // dealer follows rule of 17
                if(dealer.ShouldHit() == true) {
                    dealer.GetHand()->AddCard(DealCard());
                    Sleep(0.5);
                } else {
                    dealer.Stand();
                }
            } else {
                gameOver = 1;
            }
        }
        
        LCD.Update();
    }
    
    // determine winner
    int playerValue = player.GetHand()->GetValue();
    int dealerValue = dealer.GetHand()->GetValue();
    int result = DetermineWinner(playerValue, dealerValue);
    
    // update win loss tracking
    if(result == playerwin) {
        total_wins = total_wins + 1;
    } else {
        if(result == playerloss) {
            total_losses = total_losses + 1;
        }
    }
    
    // update player money based on result
    if(result == playerwin) {
        player.AddMoney(win_reward);
    } else {
        if(result == playerloss) {
            player.AddMoney(-loss_penalty);
        }
    }
    
    // sync global money variable
    player_money = player.GetMoney();
    
    // show loss screen with quote
    if(result == playerloss) {
        LCD.Clear(BLACK);
        
        // select random quote
        FEHRandom Random;
        int randomNum = Random.RandInt();
        int quoteIndex = randomNum % 10;
        
        // display quote
        LCD.SetFontColor(YELLOW);
        LCD.SetFontScale(0.6);
        const char* quote = loss_quotes[quoteIndex];
        LCD.WriteAt(quote, 30, 120);
        LCD.SetFontScale(1.0);
        
        LCD.Update();
        
        // wait for touch input
        float touch_x, touch_y;
        while(!LCD.Touch(&touch_x, &touch_y)) {
            LCD.Update();
        }
        while(LCD.Touch(&touch_x, &touch_y)) {
            LCD.Update();
        }
        
        return main_menu_state;
    }
    
    // show win or tie result screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    
    if(result == playerwin) {
        LCD.WriteAt("You Win!", 80, 80);
        LCD.WriteAt("+100 Money", 70, 110);
    } else {
        LCD.WriteAt("It's a Tie!", 70, 80);
        LCD.WriteAt("No Change", 70, 110);
    }
    
    char finalPlayerVal[15];
    sprintf(finalPlayerVal, "Player: %d", playerValue);
    LCD.WriteAt(finalPlayerVal, 10, 140);
    
    char finalDealerVal[15];
    sprintf(finalDealerVal, "Dealer: %d", dealerValue);
    LCD.WriteAt(finalDealerVal, 10, 160);
    
    LCD.WriteAt("Touch to continue", 30, 200);
    LCD.Update();
    
    float touch_x, touch_y;
    while(!LCD.Touch(&touch_x, &touch_y)) {
        LCD.Update();
    }
    while(LCD.Touch(&touch_x, &touch_y)) {
        LCD.Update();
    }
    
    return main_menu_state;
}

/*
 CharacterSelect Function
 
 This function allows the player to select their character (Boy or Girl).
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - world_state: After character selection
   - main_menu_state: If Back pressed
 
 Author: Kerem Cakmak
 */
int CharacterSelect() {
    LCD.Clear(BLACK);
    
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Select Character", 60, 30);
    
    Button buttons[3];
    buttons[0].SetPosition(50, 80);
    buttons[0].SetSize(100, 50);
    buttons[0].SetText("Boy");
    
    buttons[1].SetPosition(50, 140);
    buttons[1].SetSize(100, 50);
    buttons[1].SetText("Girl");
    
    buttons[2].SetPosition(50, 200);
    buttons[2].SetSize(100, 30);
    buttons[2].SetText("Back");
    
    if(character_sprites_loaded == 1) {
        boy_sprite.Draw(160, 85);
        girl_sprite.Draw(160, 145);
    }
    
    int i = 0;
    while(i < 3) {
        Blackjackbutton(buttons[i]);
        i = i + 1;
    }
    
    LCD.Update();
    
    while(1) {
        int pressed = animation(buttons, 3);
        if(pressed == 0) {
            // boy selected
            selected_character = 1;
            return world_state;
        } else {
            if(pressed == 1) {
                // girl selected
                selected_character = 2;
                return world_state;
            } else {
                if(pressed == 2) {
                    return main_menu_state;
                }
            }
        }
        LCD.Update();
    }
}

/*
 World Function
 
 This function displays the open world with movable character and touch areas
 on building doors (Casino, Shop, Bank).
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - mode_select_state: If Casino door touched
   - shop_screen_state: If Shop door touched
   - bank_state: If Bank door touched
   - main_menu_state: If Back button pressed
 
 Author: Akshit Jalli, Kerem Cakmak
 */
int World() {
    // define touch areas for building doors
    int casino_x = 100;
    int casino_y = 155;
    int casino_w = 42;
    int casino_h = 40;
    
    int shop_x = 138;
    int shop_y = 120;
    int shop_w = 50;
    int shop_h = 40;
    
    int bank_x = 184;
    int bank_y = 151;
    int bank_w = 50;
    int bank_h = 40;
    
    Button backButton;
    backButton.SetPosition(10, 10);
    backButton.SetSize(60, 25);
    backButton.SetText("Back");
    
    while(1) {
        LCD.Clear(BLACK);
        
        // draw world background
        if(world_background_loaded == 1) {
            world_background.Draw(0, 0);
        }
        
        // draw selected character sprite
        if(character_sprites_loaded == 1 && selected_character > 0) {
            if(selected_character == 1) {
                boy_sprite.Draw(player_x, player_y);
            } else {
                girl_sprite.Draw(player_x, player_y);
            }
        }
        
        Blackjackbutton(backButton);
        
        LCD.Update();
        
        // handle touch input
        float touch_x, touch_y;
        if(LCD.Touch(&touch_x, &touch_y)) {
            while(LCD.Touch(&touch_x, &touch_y)) {
                LCD.Update();
            }
            
            // check back button
            if(touch_x >= backButton.GetX() && touch_x <= backButton.GetX() + backButton.GetW() &&
               touch_y >= backButton.GetY() && touch_y <= backButton.GetY() + backButton.GetH()) {
                return main_menu_state;
            }
            
            // check casino door
            if(touch_x >= casino_x && touch_x <= casino_x + casino_w &&
               touch_y >= casino_y && touch_y <= casino_y + casino_h) {
                return mode_select_state;
            }
            
            // check shop door
            if(touch_x >= shop_x && touch_x <= shop_x + shop_w &&
               touch_y >= shop_y && touch_y <= shop_y + shop_h) {
                return shop_screen_state;
            }
            
            // check bank door
            if(touch_x >= bank_x && touch_x <= bank_x + bank_w &&
               touch_y >= bank_y && touch_y <= bank_y + bank_h) {
                return bank_state;
            }
            
            // move character to touch position
            player_x = (int)touch_x - 10;
            player_y = (int)touch_y - 10;
            
            // keep character within screen bounds
            if(player_x < 0) player_x = 0;
            if(player_x > 300) player_x = 300;
            if(player_y < 0) player_y = 0;
            if(player_y > 220) player_y = 220;
        }
        
        LCD.Update();
    }
}

/*
 Bank Function
 
 This function displays the player's current balance.
 It also has an easter egg where typing "~" gives 100,000 dollars.
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - world_state: Returns to world after viewing balance
 
 Author: Kerem Cakmak
 */
int Bank() {
    LCD.Clear(BLACK);
    
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("BANK", 120, 30);
    LCD.WriteAt("Balance:", 80, 80);
    
    // display money in yellow
    LCD.SetFontColor(YELLOW);
    LCD.SetFontScale(1.5);
    char moneyStr[20];
    sprintf(moneyStr, "$%d", player_money);
    LCD.WriteAt(moneyStr, 100, 120);
    LCD.SetFontScale(1.0);
    
    Button backButton;
    backButton.SetPosition(100, 180);
    backButton.SetSize(120, 40);
    backButton.SetText("Back to World");
    
    Blackjackbutton(backButton);
    LCD.Update();
    
    FEHKeyboard Keyboard;
    
    while(1) {
        // check for easter egg keyboard input
        char lastChar = Keyboard.lastChar();
        if(lastChar == '~') {
            // easter egg activated
            player_money = player_money + 100000;
            
            LCD.Clear(BLACK);
            LCD.SetFontColor(GREEN);
            LCD.WriteAt("EASTER EGG!", 80, 80);
            LCD.SetFontColor(YELLOW);
            LCD.WriteAt("+$100,000!", 90, 120);
            LCD.Update();
            Sleep(2.0);
            
            return bank_state;
        }
        
        // check for back button press
        int pressed = animation(&backButton, 1);
        if(pressed == 0) {
            return world_state;
        }
        LCD.Update();
    }
}


/*
 Shop Function
 
 This function displays the shop screen with navigation arrows, item descriptions,
 and shopfinal.png background. It shows one item at a time with buy functionalty.
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - world_state: Returns to world after shopping
 
 Author: Akshit Jalli, Kerem Cakmak
 */
int Shop() {
    int current_item = 0;
    int num_items = 9;
    
    Button backButton;
    backButton.SetPosition(10, 10);
    backButton.SetSize(60, 25);
    backButton.SetText("Back");
    
    // arrow positions in taskbar
    int left_arrow_x = 5;
    int left_arrow_y = 180;
    
    int right_arrow_x = 279;
    int right_arrow_y = 180;
    
    Button buyButton;
    buyButton.SetPosition(100, 100);
    buyButton.SetSize(120, 25);
    buyButton.SetText("Buy");
    
    while(1) {
        LCD.Clear(BLACK);
        
        // draw shop background
        if(shop_images_loaded == 1) {
            shop_background.Draw(0, 0);
        }
        
        // draw bottom taskbar
        int taskbarY = 130;
        int taskbarHeight = 110;
        LCD.SetFontColor(BLACK);
        LCD.FillRectangle(0, taskbarY, 319, taskbarHeight);
        LCD.SetFontColor(WHITE);
        LCD.DrawRectangle(0, taskbarY, 319, taskbarHeight);
        LCD.DrawRectangle(1, taskbarY + 1, 317, taskbarHeight - 2);
        
        // draw item image on left side of taskbar
        if(shop_images_loaded == 1) {
            shop_item_images[current_item].Draw(10, taskbarY + 5);
        }
        
        // draw navigation arrows
        if(shop_images_loaded == 1) {
            arrow_left.Draw(5, taskbarY + 50);
            arrow_right.Draw(279, taskbarY + 50);
        }
        
        // display item name
        LCD.SetFontColor(RED);
        LCD.SetFontScale(0.8);
        LCD.WriteAt(shop_items[current_item].name, 80, taskbarY + 5);
        
        // display item description
        LCD.SetFontColor(CYAN);
        LCD.SetFontScale(0.6);
        LCD.WriteAt(item_descriptions[current_item], 80, taskbarY + 25);
        
        // display item price
        char priceStr[15];
        sprintf(priceStr, "Price: $%d", shop_items[current_item].cost);
        LCD.SetFontColor(YELLOW);
        LCD.SetFontScale(0.7);
        LCD.WriteAt(priceStr, 80, taskbarY + 45);
        
        // display item counter
        char counterStr[10];
        sprintf(counterStr, "%d/%d", current_item + 1, num_items);
        LCD.SetFontColor(WHITE);
        LCD.SetFontScale(0.6);
        LCD.WriteAt(counterStr, 80, taskbarY + 65);
        
        // display player money
        char moneyStr[20];
        sprintf(moneyStr, "$%d", player_money);
        LCD.SetFontColor(YELLOW);
        LCD.SetFontScale(0.7);
        LCD.WriteAt(moneyStr, 80, taskbarY + 85);
        
        LCD.SetFontScale(1.0);
        
        // draw buy button if player can afford item
        if(player_money >= shop_items[current_item].cost) {
            Blackjackbutton(buyButton);
        } else {
            // gray out button if cannot afford
            LCD.SetFontColor(DARKGRAY);
            LCD.FillRectangle(buyButton.GetX(), buyButton.GetY(), buyButton.GetW(), buyButton.GetH());
            LCD.SetFontColor(WHITE);
            LCD.DrawRectangle(buyButton.GetX(), buyButton.GetY(), buyButton.GetW(), buyButton.GetH());
            LCD.SetFontScale(0.6);
            int textlength = strlen(buyButton.GetText());
            int textx = buyButton.GetX() + (buyButton.GetW() / 2) - (3 * textlength);
            int texty = buyButton.GetY() + (buyButton.GetH() / 2) - 5;
            LCD.WriteAt(buyButton.GetText(), textx, texty);
            LCD.SetFontScale(1.0);
        }
        
        Blackjackbutton(backButton);
        
        LCD.Update();
        
        // handle touch input
        float touch_x, touch_y;
        if(LCD.Touch(&touch_x, &touch_y)) {
            while(LCD.Touch(&touch_x, &touch_y)) {
                LCD.Update();
            }
            
            // check back button
            if(touch_x >= backButton.GetX() && touch_x <= backButton.GetX() + backButton.GetW() &&
               touch_y >= backButton.GetY() && touch_y <= backButton.GetY() + backButton.GetH()) {
                return world_state;
            }
            
            // check left arrow
            if(touch_x >= left_arrow_x && touch_x <= left_arrow_x + 30 &&
               touch_y >= left_arrow_y && touch_y <= left_arrow_y + 30) {
                current_item = current_item - 1;
                if(current_item < 0) {
                    current_item = num_items - 1;
                }
            }
            
            // check right arrow
            if(touch_x >= right_arrow_x && touch_x <= right_arrow_x + 40 &&
               touch_y >= right_arrow_y && touch_y <= right_arrow_y + 40) {
                current_item = current_item + 1;
                if(current_item >= num_items) {
                    current_item = 0;
                }
            }
            
            // check buy button
            if(touch_x >= buyButton.GetX() && touch_x <= buyButton.GetX() + buyButton.GetW() &&
               touch_y >= buyButton.GetY() && touch_y <= buyButton.GetY() + buyButton.GetH()) {
                if(player_money >= shop_items[current_item].cost) {
                    // deduct money and add to inventory
                    player_money = player_money - shop_items[current_item].cost;
                    if(inventory_items < max_inventory) {
                        inventory[inventory_items] = shop_items[current_item];
                        inventory_items = inventory_items + 1;
                    }
                    // show purchase confirmation
                    LCD.SetFontColor(GREEN);
                    LCD.WriteAt("Purchased!", 100, 160);
                    LCD.Update();
                    Sleep(0.5);
                }
            }
        }
        
        LCD.Update();
    }
}

/*
 Inventory Function
 
 This function displays the player's inventory of purchased items with images.
 It shows all owned items in a scrollable view with navigation arrows.
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - main_menu_state: Returns to main menu
 
 Author: Kerem Cakmak
 */
int Inventory() {
    int current_item = 0;
    
    Button backButton;
    backButton.SetPosition(10, 10);
    backButton.SetSize(60, 25);
    backButton.SetText("Back");
    
    // arrow positions
    int left_arrow_x = 20;
    int left_arrow_y = 110;
    
    int right_arrow_x = 280;
    int right_arrow_y = 110;
    
    while(1) {
        LCD.Clear(BLACK);
        
        // display inventory title
        LCD.SetFontColor(WHITE);
        LCD.SetFontScale(0.8);
        LCD.WriteAt("INVENTORY", 100, 5);
        LCD.SetFontScale(1.0);
        
        // check if inventory is empty
        if(inventory_items == 0) {
            LCD.SetFontColor(WHITE);
            LCD.WriteAt("No items yet!", 90, 100);
            LCD.WriteAt("Visit the shop!", 80, 130);
            
            Blackjackbutton(backButton);
            LCD.Update();
            
            int pressed = animation(&backButton, 1);
            if(pressed == 0) {
                return main_menu_state;
            }
            LCD.Update();
            continue;
        }
        
        // display item name
        if(current_item < inventory_items) {
            LCD.SetFontColor(RED);
            LCD.SetFontScale(1.0);
            LCD.WriteAt(inventory[current_item].name, 20, 35);
        }
        
        // display item image
        if(shop_images_loaded == 1 && current_item < inventory_items) {
            // find item index in shop items array
            int shopIndex = -1;
            int i = 0;
            while(i < 9) {
                if(shop_items[i].id == inventory[current_item].id) {
                    shopIndex = i;
                    break;
                }
                i = i + 1;
            }
            
            if(shopIndex >= 0) {
                shop_item_images[shopIndex].Draw(120, 60);
            }
        }
        
        // draw navigation arrows if more than one item
        if(inventory_items > 1 && shop_images_loaded == 1) {
            arrow_left.Draw(left_arrow_x, left_arrow_y);
            arrow_right.Draw(right_arrow_x, right_arrow_y);
        }
        
        // display item counter
        LCD.SetFontColor(BLACK);
        LCD.FillRectangle(140, 195, 40, 16);
        LCD.SetFontColor(WHITE);
        LCD.SetFontScale(0.7);
        char counterStr[10];
        sprintf(counterStr, "%d/%d", current_item + 1, inventory_items);
        LCD.WriteAt(counterStr, 145, 200);
        LCD.SetFontScale(1.0);
        
        Blackjackbutton(backButton);
        
        LCD.Update();
        
        // handle touch input
        float touch_x, touch_y;
        if(LCD.Touch(&touch_x, &touch_y)) {
            while(LCD.Touch(&touch_x, &touch_y)) {
                LCD.Update();
            }
            
            // check back button
            if(touch_x >= backButton.GetX() && touch_x <= backButton.GetX() + backButton.GetW() &&
               touch_y >= backButton.GetY() && touch_y <= backButton.GetY() + backButton.GetH()) {
                return main_menu_state;
            }
            
            // check navigation arrows
            if(inventory_items > 1) {
                if(touch_x >= left_arrow_x && touch_x <= left_arrow_x + 30 &&
                   touch_y >= left_arrow_y && touch_y <= left_arrow_y + 30) {
                    current_item = current_item - 1;
                    if(current_item < 0) {
                        current_item = inventory_items - 1;
                    }
                }
                
                if(touch_x >= right_arrow_x && touch_x <= right_arrow_x + 30 &&
                   touch_y >= right_arrow_y && touch_y <= right_arrow_y + 30) {
                    current_item = current_item + 1;
                    if(current_item >= inventory_items) {
                        current_item = 0;
                    }
                }
            }
        }
        
        LCD.Update();
    }
}

/*
 Credits Function
 
 This function displays the credits screen with author names.
 It shows who created the game and allows the user to return to main menu.
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - main_menu_state: Returns to main menu when Back button pressed
 
 Author: Kerem Cakmak
 */
int Credits() {
    LCD.Clear(BLACK);
    
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Credits", 100, 30);
    
    LCD.SetFontColor(CYAN);
    LCD.WriteAt("Created by:", 100, 70);
    
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Akshit Jalli", 100, 100);
    LCD.WriteAt("Kerem Cakmak", 100, 130);
    
    Button backButton;
    backButton.SetPosition(90, 180);
    backButton.SetSize(140, 35);
    backButton.SetText("Back");
    
    Blackjackbutton(backButton);
    LCD.Update();
    
    while(1) {
        int pressed = animation(&backButton, 1);
        if(pressed == 0) {
            return main_menu_state;
        }
        LCD.Update();
    }
}

/*
 Statistics Function
 
 This function displays the player's win/loss statistics.
 It shows the same information as ModeSelect but in a dedicated screen.
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - main_menu_state: Returns to main menu when Back button pressed
 
 Author: Akshit Jalli, Kerem Cakmak
 */
int Statistics() {
    LCD.Clear(BLACK);
    
    // draw bottom taskbar
    int taskbarY = 200;
    int taskbarHeight = 40;
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(0, taskbarY, 319, taskbarHeight);
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(0, taskbarY, 319, taskbarHeight);
    LCD.DrawRectangle(1, taskbarY + 1, 317, taskbarHeight - 2);
    
    // display title
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("STATISTICS", 100, 30);
    
    // display wins in taskbar
    LCD.SetFontColor(GREEN);
    LCD.SetFontScale(0.6);
    char winStr[15];
    sprintf(winStr, "Wins: %d", total_wins);
    LCD.WriteAt(winStr, 10, taskbarY + 12);
    
    // display losses in taskbar
    LCD.SetFontColor(RED);
    LCD.SetFontScale(0.6);
    char lossStr[15];
    sprintf(lossStr, "Loss: %d", total_losses);
    LCD.WriteAt(lossStr, 150, taskbarY + 12);
    LCD.SetFontScale(1.0);
    
    Button backButton;
    backButton.SetPosition(90, 160);
    backButton.SetSize(140, 35);
    backButton.SetText("Back");
    
    Blackjackbutton(backButton);
    LCD.Update();
    
    while(1) {
        int pressed = animation(&backButton, 1);
        if(pressed == 0) {
            return main_menu_state;
        }
        LCD.Update();
    }
}

/*
 Instructions Function
 
 This function displays game instructions in a simple bullet format.
 
 Input Arguments: None
 
 Return Value: Integer representing next game state
   - main_menu_state: Returns to main menu when Back button pressed
 
 Author: Akshit Jalli, Kerem Cakmak
 */
int Instructions() {
    LCD.Clear(BLACK);
    
    // display title
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("INSTRUCTIONS", 90, 10);
    
    // draw box around instructions
    int boxX = 5;
    int boxY = 30;
    int boxW = 310;
    int boxH = 145;
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(boxX, boxY, boxW, boxH);
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(boxX, boxY, boxW, boxH);
    LCD.DrawRectangle(boxX + 1, boxY + 1, boxW - 2, boxH - 2);
    
    // display instructions with small font inside box
    LCD.SetFontColor(WHITE);
    LCD.SetFontScale(0.5);
    
    LCD.WriteAt("- Get cards close to 21", 15, 40);
    LCD.WriteAt("- Dealer must hit on 16", 15, 55);
    LCD.WriteAt("- Win: +$100, Loss: -$50", 15, 70);
    LCD.WriteAt("- Use Hit to get cards", 15, 85);
    LCD.WriteAt("- Use Stand to stop", 15, 100);
    LCD.WriteAt("- Over 21 is a bust", 15, 115);
    LCD.WriteAt("- Visit shop to buy items", 15, 130);
    LCD.WriteAt("- Bank shows your balance", 15, 145);
    
    LCD.SetFontScale(1.0);
    
    Button backButton;
    backButton.SetPosition(90, 180);
    backButton.SetSize(140, 35);
    backButton.SetText("Back");
    
    Blackjackbutton(backButton);
    LCD.Update();
    
    while(1) {
        int pressed = animation(&backButton, 1);
        if(pressed == 0) {
            return main_menu_state;
        }
        LCD.Update();
    }
}

/*
 QuitGame Function
 
 This function displays a quit message and ends the game loop.
 It promotes responsible gaming with an anti-gambling message.
 
 Input Arguments: None
 
 Return Value: None (void)
 
 Author: Kerem Cakmak
 */
void QuitGame() {
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Happy Holidays!", 70, 100);
    LCD.WriteAt("Don't Gamble...", 70, 200);
    LCD.Update();
    Sleep(2.0);
    game_on = 0;
}

// function prototypes
int Statistics();
int Instructions();

/*
 main Function
 
It initializes game state,
 loads all resources like images, and runs the main game loop.
 
 Input Arguments: None (standard main function)
 
 Return Value: Integer exit code (0 for success)
 
 Author: Kerem Cakmak
 */
int main(){
    player_money = starting_money;
    inventory_items = 0;
    game_on = 1;
    total_wins = 0;
    total_losses = 0;
    
    // initialize session timer
    session_start_time = TimeNow();
    
    LoadMainMenuImage();
    LoadGameImages();
    LoadWorldImages();
    int current_state = main_menu_state;
    while(game_on == 1){
        if(current_state == main_menu_state){
            current_state = MainMenu();
        } else {
            if(current_state == mode_select_state){
                current_state = ModeSelect();
            } else {
                if(current_state == character_select_state){
                    current_state = CharacterSelect();
                } else {
                    if(current_state == world_state){
                        current_state = World();
                    } else {
                        if(current_state == blackjack_state){
                            current_state = PlayBlackjack();
                        } else {
                            if(current_state == shop_screen_state){
                                current_state = Shop();
                            } else {
                                if(current_state == bank_state){
                                    current_state = Bank();
                                } else {
                                if(current_state == inventory_state){
                                    current_state = Inventory();
                                } else {
                                if(current_state == credits_state){
                                    current_state = Credits();
                                } else {
                                    if(current_state == statistics_state){
                                        current_state = Statistics();
                                    } else {
                                        if(current_state == instructions_state){
                                            current_state = Instructions();
                                        } else {
                                            if(current_state == quit){
                                                QuitGame();
                                                break;
                                            }
                                        }
                                    }
                                }
                                }
                                }
                            }
                        }
                    }
                }
            }
        }
        LCD.Update();
    }
    return 0;
}