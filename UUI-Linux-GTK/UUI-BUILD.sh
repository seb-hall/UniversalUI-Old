


g++ main.cpp CoreHost.cpp  AngeloText.cpp Angelo.cpp -o main $(pkg-config --cflags gtk+-3.0 --libs) -lepoxy -lfreetype -I./include
