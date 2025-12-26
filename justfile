install: update_config
    sudo make install

update_config:
    sudo cp config.def.h config.h
