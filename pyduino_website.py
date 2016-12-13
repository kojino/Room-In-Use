from flask import Flask, render_template,request, redirect, url_for, jsonify
from pyduino import *
import time

app = Flask(__name__)

# initialize connection to Arduino
# if your arduino was running on a serial port other than '/dev/ttyACM0/'
# declare: a = Arduino(serial_port='/dev/ttyXXXX')
a = Arduino()
time.sleep(3)

# declare the pins we're using
MOTION_SENSOR = 2
GREEN = 4
RED = 8

# initialize the digital pin as output
# a.set_pin_mode(GREEN,'O')
# a.set_pin_mode(RED, 'O')
# a.set_pin_mode(MOTION_SENSOR, 'I')

print 'Arduino initialized'

# we are able to make 2 different requests on our webpage
# GET = we just type in the url
# POST = some sort of form submission like a button
@app.route('/', methods = ['POST','GET'])
def hello_world():
    input_ = a.room_read()
    if input_ == "empty":
        return render_template('index.html', is_used = False)
    elif input_ == "use":
        return render_template('index.html', is_used = True)
#
@app.route('/refresh', methods=['GET'])
def refresh_data():
    # read in analog value from photoresistor
    time.sleep(10)
    input_ = a.room_read()
    if input_ == "empty":
        return jsonify("empty")
    elif input_ == "use":
        return jsonify("in use")
    else:
        return

if __name__ == "__main__":

    # lets launch our webpage!
    # do 0.0.0.0 so that we can log into this webpage
    # using another computer on the same network later
    app.run(host='0.0.0.0',threaded=True)
