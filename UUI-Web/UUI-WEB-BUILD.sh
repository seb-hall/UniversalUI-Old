emcc testing.cpp -lGL -lGLU -lglut  -s WASM=1 -s LEGACY_GL_EMULATION=1 -O3 -o ./Build/index.js
sudo cp ./Build/index.html /var/www/html/
sudo cp ./Build/index.js /var/www/html/
sudo cp ./Build/index.wasm /var/www/html/