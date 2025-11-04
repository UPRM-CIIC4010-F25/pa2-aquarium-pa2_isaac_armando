#include "Aquarium.h"
#include <cstdlib>


string AquariumCreatureTypeToString(AquariumCreatureType t){
    switch(t){
        case AquariumCreatureType::MonsterCreature:
            return "MonsterCreature";
        case AquariumCreatureType::PinkFish:
            return "PinkFish";
        case AquariumCreatureType::TopFish:
            return "TopHatFish";
        case AquariumCreatureType::GreenFish:
            return "GreenFish";
        case AquariumCreatureType::BiggerFish:
            return "BiggerFish";
        case AquariumCreatureType::NPCreature:
            return "BaseFish";
        case AquariumCreatureType::PlayerCreature:
            return "PlayerFish";
        default:
            return "UknownFish";
    }
}

// PlayerCreature Implementation
PlayerCreature::PlayerCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, speed, 10.0f, 1, sprite) {}


void PlayerCreature::setDirection(float dx, float dy) {
    m_dx = dx;
    m_dy = dy;
    normalize();
}

void PlayerCreature::move() {
    m_x += m_dx * m_speed;
    m_y += m_dy * m_speed;
    this->bounce();
}

void PlayerCreature::reduceDamageDebounce() {
    if (m_damage_debounce > 0) {
        --m_damage_debounce;
    }
}

void PlayerCreature::update() {
    this->reduceDamageDebounce();
    this->move();
}


void PlayerCreature::draw() const {
    
    ofLogVerbose() << "PlayerCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    if (this->m_damage_debounce > 0) {
        ofSetColor(ofColor::red); // Flash red if in damage debounce
    }
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
    ofSetColor(ofColor::white); // Reset color

}

void PlayerCreature::changeSpeed(int speed) {
    m_speed = speed;
}

void PlayerCreature::loseLife(int debounce) {
    if (m_damage_debounce <= 0) {
        if (m_lives > 0) this->m_lives -= 1;
        m_damage_debounce = debounce; // Set debounce frames
        ofLogNotice() << "Player lost a life! Lives remaining: " << m_lives << std::endl;
    }
    // If in debounce period, do nothing
    if (m_damage_debounce > 0) {
        ofLogVerbose() << "Player is in damage debounce period. Frames left: " << m_damage_debounce << std::endl;
    }
}

// NPCreature Implementation
NPCreature::NPCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, speed, 30, 1, sprite) {
    m_dx = (rand() % 3 - 1); // -1, 0, or 1
    m_dy = (rand() % 3 - 1); // -1, 0, or 1
    normalize();

    m_creatureType = AquariumCreatureType::NPCreature;
}

void NPCreature::move() {
    // Simple AI movement logic (random direction)
    m_x += m_dx * m_speed;
    m_y += m_dy * m_speed;
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }
    bounce();
}

void NPCreature::draw() const {
    ofLogVerbose() << "NPCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    ofSetColor(ofColor::white);
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
}


BiggerFish::BiggerFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(60); // Bigger fish have a larger collision radius
    m_value = 5; // Bigger fish have a higher value
    m_creatureType = AquariumCreatureType::BiggerFish;
}

void BiggerFish::move() {
    // Bigger fish might move slower or have different logic
    m_x += m_dx * (m_speed * 0.5); // Moves at half speed
    m_y += m_dy * (m_speed * 0.5);
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }

    bounce();
}

void BiggerFish::draw() const {
    ofLogVerbose() << "BiggerFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);
}

Bomb::Bomb(float x, float y, std::shared_ptr<GameSprite> sprite) : Creature(x,y,0.0,45.0,-1000,sprite){}
void Bomb :: move(){}
void Bomb :: draw () const {
    if(m_sprite)
    m_sprite->draw(m_x, m_y);
} 

Seaweed::Seaweed(float x, float y, std::shared_ptr<GameSprite> sprite) : Creature(x, y, 0.0, 45.0, 100, sprite) {}  
void Seaweed :: draw () const {
    if(m_sprite)
    m_sprite->draw(m_x, m_y);
} 
void Seaweed :: move(){}


GreenFish::GreenFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite):
NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(50); // Green fish will have a collision radius smaller than BiggerFish
    m_value = 4; //  lower value than Bigger fish
    m_creatureType = AquariumCreatureType::GreenFish;
}
void GreenFish::move(){
    m_x += m_dx * (m_speed * 1.4); // Moves at faster speed
    m_y += m_dy * (m_speed * 1.4);
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }

    bounce();}

void GreenFish::draw() const { 
    ofLogVerbose() << "GreenFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);
}


TopFish::TopFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite):
NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(60); // Top fish will have a collision radius equal to BiggerFish
    m_value = 8; // higher value
    m_creatureType = AquariumCreatureType::TopFish;
}
void TopFish::move(){
    m_x += m_dx * (m_speed ); // Moves at normal speed
    m_y += m_dy * (m_speed );
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);  
        if(rand() % 3 == 0){                    //Randomly flips around
            this->m_sprite->setFlipped(true);
        }
    }

    bounce();}
void TopFish::draw() const { 
    ofLogVerbose() << "TopFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);
}

PinkFish::PinkFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite):
NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(50); // Pink fish will have a smaller collision radius
    m_value = 14; // higher value
    m_creatureType = AquariumCreatureType::PinkFish;
}
void PinkFish::move(){
    m_x += m_dx * (m_speed * 1.5); // Moves at a bit faster speed than Green_fish
    m_y += m_dy * (m_speed * 1.5);
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }

    bounce();
}

void PinkFish::draw() const { 
    ofLogVerbose() << "PinkFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);
}

MonsterCreature::MonsterCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite):NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(100); // MonsterCreature will have the largest collision radius
    m_value = 300; // highest value
    m_creatureType = AquariumCreatureType::MonsterCreature;
}
void MonsterCreature::move(){
    m_x += m_dx * (m_speed * 1.3); // Moves at slightly faster speed
    m_y += m_dy * (m_speed * 1.3);
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }

    bounce();
}
void MonsterCreature::draw() const { 
    ofLogVerbose() << "MonsterCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);
}


// AquariumSpriteManager
AquariumSpriteManager::AquariumSpriteManager(){
    this->m_player_fish = std::make_shared<GameSprite>("Gold_fish.png", 70,70);  //Changed the player's fish sprite
    this->m_npc_fish = std::make_shared<GameSprite>("base-fish.png", 70,70);
    this->m_big_fish = std::make_shared<GameSprite>("bigger-fish.png", 120, 120);
    this->m_green_fish = std::make_shared<GameSprite>("cust_green_fish.png", 100, 100);
    this->m_top_fish = std::make_shared<GameSprite>("Top_fish.png", 120, 120);
    this->m_pink_fish = std::make_shared<GameSprite>("Pink_fish.png", 90, 90);
    this->m_monster_creature = std::make_shared<GameSprite>("Pix_Mons.png", 470, 470);
    this->m_bomb = std::make_shared<GameSprite>("bombs.png", 48, 45 );
    this->m_seaweed = std::make_shared<GameSprite>("seaweed.png", 72, 72 );
}

std::shared_ptr<GameSprite> AquariumSpriteManager::GetSprite(AquariumCreatureType t){
    switch(t){
        case AquariumCreatureType::MonsterCreature:
            return std::make_shared<GameSprite>(*this->m_monster_creature);
        case AquariumCreatureType::PinkFish:
            return std::make_shared<GameSprite>(*this->m_pink_fish);
        case AquariumCreatureType::TopFish:
            return std::make_shared<GameSprite>(*this->m_top_fish);
        case AquariumCreatureType::GreenFish:
            return std::make_shared<GameSprite>(*this->m_green_fish);
        case AquariumCreatureType::BiggerFish:
            return std::make_shared<GameSprite>(*this->m_big_fish);   
        case AquariumCreatureType::NPCreature:
            return std::make_shared<GameSprite>(*this->m_npc_fish);
        case AquariumCreatureType::PlayerCreature:
            return std::make_shared<GameSprite>(*this->m_player_fish);
        case AquariumCreatureType::Bomb:
            return std::make_shared<GameSprite>(*this->m_bomb);
                case AquariumCreatureType::Seaweed:
            return std::make_shared<GameSprite>(*this->m_seaweed);
        default:
            return nullptr;
    }
}


// Aquarium Implementation
Aquarium::Aquarium(int width, int height, std::shared_ptr<AquariumSpriteManager> spriteManager) 
    : m_width(width), m_height(height) { 
        m_sprite_manager =  spriteManager;
        bomb_sound.load("bomb.mp3");                            //Bomb
        bomb_sound.setVolume(1.0f);
        seaweed_sound.load("Live_sound.mp3");                   //Seaweed        
        seaweed_sound.setVolume(1.0f);
    }



void Aquarium::addCreature(std::shared_ptr<Creature> creature) {
    creature->setBounds(m_width - 20, m_height - 20);
    m_creatures.push_back(creature);
}

void Aquarium::addAquariumLevel(std::shared_ptr<AquariumLevel> level){
    if(level == nullptr){return;} // guard to not add noise
    this->m_aquariumlevels.push_back(level);
}

void Aquarium::update() {
    for (auto& creature : m_creatures) {
        creature->move();
    }
    if ((rand() % 1000) < 6) {
    SpawnBomb();
}
    this->Repopulate();
}

void Aquarium::draw() const {
    for (const auto& creature : m_creatures) {
        creature->draw();
    }
}


void Aquarium::removeCreature(std::shared_ptr<Creature> creature) {
    auto it = std::find(m_creatures.begin(), m_creatures.end(), creature);
    if (it != m_creatures.end()) {
        ofLogVerbose() << "removing creature " << endl;
        int selectLvl = this->currentLevel % this->m_aquariumlevels.size();
        auto npcCreature = std::static_pointer_cast<NPCreature>(creature);
        this->m_aquariumlevels.at(selectLvl)->ConsumePopulation(npcCreature->GetType(), npcCreature->getValue());
        m_creatures.erase(it);
    }
}

void Aquarium::removeBomb(std::shared_ptr<Creature> creature) {
    auto it = std::find(m_creatures.begin(), m_creatures.end(), creature);
    if (it != m_creatures.end()) {
        m_creatures.erase(it);
    }
}
int Aquarium::KillFishes() {
    int removed = 0;
    for(int i = m_creatures.size() - 1; i >= 0 && removed < 3; i--){
        if(m_creatures[i]->getValue() > 0){ 
            removeCreature(m_creatures[i]);
            removed++;
        }
    } return removed;
}
void Aquarium::clearCreatures() {
    m_creatures.clear();
}

std::shared_ptr<Creature> Aquarium::getCreatureAt(int index) {
    if (index < 0 || size_t(index) >= m_creatures.size()) {
        return nullptr;
    }
    return m_creatures[index];
}



void Aquarium::SpawnCreature(AquariumCreatureType type) {
    int x = rand() % this->getWidth();
    int y = rand() % this->getHeight();
    int speed = 1 + rand() % 25; // Speed between 1 and 25

    switch (type) {
        case AquariumCreatureType::NPCreature:
            this->addCreature(std::make_shared<NPCreature>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::NPCreature)));
            break;
        case AquariumCreatureType::BiggerFish:
            this->addCreature(std::make_shared<BiggerFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::BiggerFish)));
            break;
        case AquariumCreatureType::GreenFish:
            this->addCreature(std::make_shared<GreenFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::GreenFish)));
            break;
        case AquariumCreatureType::TopFish:
            this->addCreature(std::make_shared<TopFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::TopFish)));
            break;  
        case AquariumCreatureType::PinkFish:
            this->addCreature(std::make_shared<PinkFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::PinkFish)));
            break;
        case AquariumCreatureType::MonsterCreature:
            this->addCreature(std::make_shared<MonsterCreature>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::MonsterCreature)));
            break;
        default:
            ofLogError() << "Unknown creature type to spawn!";
            break;
    }

}

void Aquarium::SpawnBomb() {
    float x = 30 + rand() % (m_width - 60);
    float y = 30 + rand() % (m_height - 60);
    this->addCreature(std::make_shared<Bomb>(x, y, this->m_sprite_manager->GetSprite(AquariumCreatureType::Bomb)));
}

void Aquarium ::SpawnSeaweed(){
    float x = 30 + rand() % (m_width - 60);
    float y = m_height - 240;
    this->addCreature(std::make_shared<Seaweed>(x, y, this->m_sprite_manager->GetSprite(AquariumCreatureType::Seaweed)));
}

// repopulation will be called from the levl class
// it will compose into aquarium so eating eats frm the pool of NPCs in the lvl class
// once lvl criteria met, we move to new lvl through inner signal asking for new lvl
// which will mean incrementing the buffer and pointing to a new lvl index
void Aquarium::Repopulate() {
    ofLogVerbose("entering phase repopulation");
    // lets make the levels circular
    int selectedLevelIdx = this->currentLevel % this->m_aquariumlevels.size();
    ofLogVerbose() << "the current index: " << selectedLevelIdx << endl;
    std::shared_ptr<AquariumLevel> level = this->m_aquariumlevels.at(selectedLevelIdx);


    if(level->isCompleted()){
        level->levelReset();
        this->currentLevel += 1;
        selectedLevelIdx = this->currentLevel % this->m_aquariumlevels.size();
        ofLogNotice()<<"new level reached : " << selectedLevelIdx << std::endl;
        level = this->m_aquariumlevels.at(selectedLevelIdx);
        this->clearCreatures();
    }

    
    // now lets find how many to respawn if needed 
    std::vector<AquariumCreatureType> toRespawn = level->Repopulate();
    ofLogVerbose() << "amount to repopulate : " << toRespawn.size() << endl;
    if(toRespawn.size() <= 0 ){return;} // there is nothing for me to do here
    for(AquariumCreatureType newCreatureType : toRespawn){
        this->SpawnCreature(newCreatureType);
    }
}


// Aquarium collision detection
std::shared_ptr<GameEvent> DetectAquariumCollisions(std::shared_ptr<Aquarium> aquarium, std::shared_ptr<PlayerCreature> player) {
    if (!aquarium || !player) return nullptr;
    
    for (int i = 0; i < aquarium->getCreatureCount(); ++i) {
        std::shared_ptr<Creature> npc = aquarium->getCreatureAt(i);
        if (npc && checkCollision(player, npc)) {
            return std::make_shared<GameEvent>(GameEventType::COLLISION, player, npc);
        }
    }
    return nullptr;
};

//  Imlementation of the AquariumScene

void AquariumGameScene::Update(){
    std::shared_ptr<GameEvent> event;

    this->m_player->update();

    if (this->updateControl.tick()) {
        event = DetectAquariumCollisions(this->m_aquarium, this->m_player);
        if (event != nullptr && event->isCollisionEvent()) {
            ofLogVerbose() << "Collision detected between player and NPC!" << std::endl;
            if(event->creatureB != nullptr){
                event->print();
                if(event->creatureB->getValue() < 0){
                int killed = this->m_aquarium->KillFishes(); 
                     if (killed > 0) {
                      this->m_player->addToScore(killed, 1);
                    }
                this->m_aquarium->bomb_sound.play();
                this->m_aquarium->removeBomb(event->creatureB);
                return;
                }
                else if (event->creatureB->getValue() == 100) {
                    this->m_player->increaselife(1);
                    this->m_aquarium->seaweed_sound.play();
                    this->m_aquarium->removeCreature(event->creatureB);
                }

                    else if(this->m_player->getPower() < event->creatureB->getValue()){
                    ofLogNotice() << "Player is too weak to eat the creature!" << std::endl;
                    this->m_player->loseLife(3*60); // 3 frames debounce, 3 seconds at 60fps
                    if(this->m_player->getLives() <= 0){
                        this->m_lastEvent = std::make_shared<GameEvent>(GameEventType::GAME_OVER, this->m_player, nullptr);
                        return;
                    }
                }
                else{
                    this->m_aquarium->removeCreature(event->creatureB);
                    this->m_player->addToScore(1, event->creatureB->getValue());
                    int currentScore = this->m_player->getScore();
                    if (currentScore / 20 > this->m_player->m_last_power) {
                        this->m_player->m_last_power = currentScore / 20;
                        this->m_player->increasePower(1);
                        ofLogNotice() << "Player power increased to " << this->m_player->getPower() << "!" << std::endl;
                        
                        if (this->m_player->getPower() % 5 == 0) {
                            this->m_player->increaselife(1);
                            ofLogNotice() << "Player life increased to " << this->m_player->getLives() << "!" << std::endl;
                        }
                    }
                    if (this->m_player->getPower() % 4 == 0) {
                    static int lastSeaweedPower = -1;
                    if (lastSeaweedPower != this->m_player->getPower()) {
                     this->m_aquarium->SpawnSeaweed();
                     lastSeaweedPower = this->m_player->getPower();
                       }
                    }
                }
                
                

            } else {
                ofLogError() << "Error: creatureB is null in collision event." << std::endl;
            }
        }
        this->m_aquarium->update();
    }

}

void AquariumGameScene::Draw() {
    this->m_player->draw();
    this->m_aquarium->draw();
    this->paintAquariumHUD();

}


void AquariumGameScene::paintAquariumHUD(){
    float panelWidth = ofGetWindowWidth() - 150;
    ofDrawBitmapString("Score: " + std::to_string(this->m_player->getScore()), panelWidth, 20);
    ofDrawBitmapString("Power: " + std::to_string(this->m_player->getPower()), panelWidth, 30);
    ofDrawBitmapString("Lives: " + std::to_string(this->m_player->getLives()), panelWidth, 40);
    for (int i = 0; i < this->m_player->getLives(); ++i) {
        ofSetColor(ofColor::red);
        ofDrawCircle(panelWidth + i * 20, 50, 5);
    }
    ofSetColor(ofColor::white); // Reset color to white for other drawings
}

void AquariumLevel::populationReset(){
    for(auto node: this->m_levelPopulation){
        node->currentPopulation = 0; // need to reset the population to ensure they are made a new in the next level
    }
}

void AquariumLevel::ConsumePopulation(AquariumCreatureType creatureType, int power){
    for(std::shared_ptr<AquariumLevelPopulationNode> node: this->m_levelPopulation){
        ofLogVerbose() << "consuming from this level creatures" << endl;
        if(node->creatureType == creatureType){
            ofLogVerbose() << "-cosuming from type: " << AquariumCreatureTypeToString(node->creatureType) <<" , currPop: " << node->currentPopulation << endl;
            if(node->currentPopulation == 0){
                return;
            } 
            node->currentPopulation -= 1;
            ofLogVerbose() << "+cosuming from type: " << AquariumCreatureTypeToString(node->creatureType) <<" , currPop: " << node->currentPopulation << endl;
            this->m_level_score += power;
            return;
        }
    }
}

bool AquariumLevel::isCompleted(){
    return this->m_level_score >= this->m_targetScore;
}

std::vector<AquariumCreatureType> AquariumLevel::Repopulate() {
    std::vector<AquariumCreatureType> toRepopulate;
    for(std::shared_ptr<AquariumLevelPopulationNode> node : this->m_levelPopulation){
        int delta = node->population - node->currentPopulation;
        ofLogVerbose() << "to Repopulate :  " << delta << endl;
        if(delta >0){
            for(int i = 0; i<delta; i++){
                toRepopulate.push_back(node->creatureType);
            }
            node->currentPopulation += delta;
        }
    }
    return toRepopulate;
}


