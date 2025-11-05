install: update_config
    sudo make install

update_config:
    cp config.def.h config.h
