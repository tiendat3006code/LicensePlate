#! /bin/bash
sudo systemctl enable license_plate_pi.service
sudo systemctl daemon-reload
sudo systemctl start license_plate_pi.service
