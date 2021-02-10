#include "SSD1309.hpp"

/*!
    @brief  Constructor for I2C-interfaced OLED display.
    @param  DevAddr
            Device i2c address shifted one to the left.
    @param  width
            Display width.
    @param  height
            Display height.
    @param  i2c
            Pointer to an existing i2c instance.
    @return SSD1306 object.
*/
SSD1309::SSD1309(uint16_t const DevAddr, uint8_t const width, uint8_t const height, i2c_inst_t * i2c, uint8_t resetPin) : DevAddr(DevAddr), width(width), height(height), i2c(i2c), resetPin(resetPin)
{
	gpio_init(this->resetPin);
	gpio_set_dir(this->resetPin, GPIO_OUT);
	gpio_pull_up(this->resetPin);
	this->reset();

	this->buffer = new unsigned char[this->width*this->height/8];
	this->displayON(0);

	this->sendCommand(SSD1309_setLowCSAinPAM);
	this->sendCommand(SSD1309_setHighCSAinPAM);
	this->sendCommand(SSD1309_setMemoryAddressingMode);  
	this->sendCommand(0x00);  
	
	this->setContrast(0xFF);

	this->sendCommand(SSD1309_inversionOff);

	this->sendCommand(SSD1309_setMultiplexRatio);
	this->sendCommand(0x3F);

	this->sendCommand(SSD1309_setDisplayOffset);
	this->sendCommand(0x00);

	this->sendCommand(SSD1309_setDisplayClockDivideRatio);
	this->sendCommand(0x80);

	this->sendCommand(SSD1309_setPreChargePeriod);
	this->sendCommand(0x22);

	this->sendCommand(SSD1309_setCOMpinsHWconfig);
	this->sendCommand(0x12);
	
	this->sendCommand(SSD1309_setVCOMHdeselectLevel);	
	this->sendCommand(0x40);

	this->sendCommand(SSD1309_followRAMcontent);

	this->displayON(1);
	this->clear();
	this->display();
}


/*!
    @brief  Deconstructor for I2C-interfaced OLED display.
*/
SSD1309::~SSD1309() 
{
	delete this->buffer;
}


/*!
 * @brief Reset display.
 *
 */
void SSD1309::reset()
{	
	gpio_put(this->resetPin, 0);
	sleep_us(5);
	gpio_put(this->resetPin, 1);
}


/*!
 * @brief Send command to display.
 *
 */
void SSD1309::sendCommand(uint8_t command)
{
	uint8_t mess[2] = {0x00, command};
	i2c_write_blocking(this->i2c, this->DevAddr, mess, 2, false);
}

/*!
 * @brief Invert colors.
 *
 */
void SSD1309::invertColors(uint8_t Invert)
{
	this->sendCommand(Invert ? SSD1309_inversionOn : SSD1309_inversionOff);
}

/*!
 * @brief Turn on display.
 * 0 – Turn OFF
 * 1 – Turn ON
 */
void SSD1309::displayON(uint8_t On)
{
	this->sendCommand(On ? SSD1309_pwrOn : SSD1309_pwrOff);
}

/*!
 * @brief Set contrast.
 *
 */
void SSD1309::setContrast(uint8_t Contrast)
{
	this->sendCommand(SSD1309_setContrastControl);
	this->sendCommand(Contrast);
}


/*!
 * @brief Draw pixel in the buffer.
 *
 */
void SSD1309::drawPixel(int16_t x, int16_t y, colors Color)
{
	 if ((x < 0) || (x >= this->width) || (y < 0) || (y >= this->height))
		 return;

	 switch(Color)
	 {
		 case colors::WHITE:   this->buffer[x+ (y/8) * this->width] |=  (1 << (y&7)); break;
		 case colors::BLACK:   this->buffer[x+ (y/8) * this->width] &= ~(1 << (y&7)); break;
		 case colors::INVERSE: this->buffer[x+ (y/8) * this->width] ^=  (1 << (y&7)); break;
	 }
}


/*!
 * @brief Clear the buffer.
 *
 */
void SSD1309::clear(colors Color)
{
	switch (Color)
	{
		case colors::WHITE:
			memset(buffer, 0xFF, (this->height * this->width / 8));
			break;
		case colors::BLACK:
			memset(buffer, 0x00, (this->height * this->width / 8));
			break;
	}
}


/*!
 * @brief Send buffer to OLED.
 *
 */
void SSD1309::display(unsigned char *data)
{
	if(data == nullptr) data = this->buffer;
	for(uint8_t i = 0; i < this->height/8; i++) {
		this->sendCommand(0xB0 + i);
		this->sendCommand(0x00);
		this->sendCommand(0x10);
		this->sendData(&data[this->width*i], this->width);
	}
}


/*!
 * @brief Send data to OLED GCRAM.
 *
 */
void SSD1309::sendData(uint8_t* buffer, size_t buff_size)
{
	unsigned char mess[buff_size+1];

	mess[0] = 0x40;
	memcpy(mess+1, buffer, buff_size);

	i2c_write_blocking(this->i2c, this->DevAddr, mess, buff_size+1, false);
}


/*!
 * @brief Return display height.
 *
 */
uint8_t SSD1309::getHeight()
{
	return this->height;
}


/*!
 * @brief Return display width.
 *
 */
uint8_t SSD1309::getWidth()
{
	return this->width;
}