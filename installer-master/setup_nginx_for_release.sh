# Generate ssl certs
sudo openssl req -newkey rsa:4096 \
            -x509 \
            -days 3650 \
            -nodes \
            -out /etc/nginx/client.pem \
            -keyout /etc/nginx/client.key

# Copy nginx config
sudo unlink /etc/nginx/sites-enabled/default
sudo cp nano_firewall_for_release.conf /etc/nginx/sites-available/nano_firewall.conf
sudo ln -s /etc/nginx/sites-available/nano_firewall.conf /etc/nginx/sites-enabled/nano_firewall.conf
sudo nginx -s reload

echo nginx configured
