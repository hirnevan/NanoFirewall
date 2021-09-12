cd web-app
npm install
sudo rm -rf ./build
npm run build
sudo mkdir /var/www/nanofirewall
sudo mkdir /var/www/nanofirewall/html
sudo cp -r ./build/* /var/www/nanofirewall/html
cd ..

echo Web app built
