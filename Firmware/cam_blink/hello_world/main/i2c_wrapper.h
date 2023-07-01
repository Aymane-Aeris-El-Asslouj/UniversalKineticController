


void i2c_master_init(uint8_t input_slave_address, uint8_t sda_pin, uint8_t scl_pin);

uint8_t i2c_read_reg(uint8_t reg);

void i2c_write_reg(uint8_t reg, uint8_t data);