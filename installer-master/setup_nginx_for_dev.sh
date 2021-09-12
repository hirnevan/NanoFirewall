sudo unlink /etc/nginx/sites-enabled/default
sudo cp nano_firewall_for_dev.conf /etc/nginx/sites-available/nano_firewall.conf
sudo ln -s /etc/nginx/sites-available/nano_firewall.conf /etc/nginx/sites-enabled/nano_firewall.conf
sudo nginx -s reload

echo nginx configured
