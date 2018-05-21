import serial
import time
import requests
import struct


class Display:
    def __init__(self, port='/dev/ttyUSB0', baud=9600):
        self.ser = serial.Serial(port, baud)

    def write_string(self, a):
        self.ser.write(a.encode(encoding='utf-8'))

    def write_byte(self, x):
        self.ser.write(struct.pack('>B', x))

    def write_brightness(self, x):
        self.write_string('\\b')
        self.write_byte(x)

    def brightness_test(self):
        for i in range(255):
            self.write_brightness(i)

        for i in reversed(range(255)):
            self.write_brightness(i)


coins = ['bitcoin', 'ethereum', 'iota']
api_url = 'https://api.coinmarketcap.com/v1/ticker/'

coins_to_show_count = 30

show_only_listed_coins = False

lcd = Display()


def main():
    lcd.brightness_test()

    while True:
        current_time = time.localtime()

        lcd.write_string('\\n' + time.strftime("%a, %d %b %Y", current_time))
        lcd.write_string('\\n' + time.strftime("%H:%M:%S", current_time))

        hour = current_time.tm_hour
        if hour < 6 or hour > 22:
            brightness = 4
        elif hour > 18:
            brightness = 127
        else:
            brightness = 255

        lcd.write_brightness(brightness)

        time.sleep(10)
        if hour < 6:
            continue

        try:
            response = requests.get(api_url)
            response_json = response.json()

            if not show_only_listed_coins:
                response_json = response_json[:coins_to_show_count]

            for coin in sorted(response_json, key=lambda x: x['symbol']):
                if not show_only_listed_coins or coin['id'] in coins:
                    price = float(coin['price_usd'])
                    symbol = coin['symbol']
                    output = '{:6} ${:.2f}'.format(symbol + ':', price)
                    lcd.write_string('\\n' + output)
                    print(output)
                    time.sleep(2)

        except Exception:
            print('Error retrieving prices. Trying again in 10 seconds')
            time.sleep(10)


if __name__ == '__main__':
    main()
