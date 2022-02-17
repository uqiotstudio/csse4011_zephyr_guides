# Sample Solutions  

This library contains sample solutions to tutorial questions and examples to implementing particular Zephyr sub-systems.

## Building and Flashing

The solutions can be built by navigating to the particular app dir. For example:
```shell
cd console_example/

west build -p -b <board_name>
west flash -r <runner> 
```

Refer to the board guides for additional instructions for building and flashing for particular boards.