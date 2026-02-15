extern "C" void main(){
    char* vga = (char*)0xb8000;
    const char* text = "Luminor OS";
    for (int i = 0; text[i] != '\0'; i++) {
        vga[i * 2] = text[i];     // character
        vga[i * 2 + 1] = 0x07;    // color
    }
    return;

}
