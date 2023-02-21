namespace LED{
    int getCurrentColorID();
    int getCurrentColorHex();
    void cycleColor();
    void startupColors();
    void updateColorStrips(std::pair<int, int> indexes, int color);
    enum COLOR_IDS{ORANGE_ID,BLUE_ID,RED_ID};
}