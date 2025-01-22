from flask import Flask, request, jsonify
import csv
from datetime import datetime
import pytz
 
app = Flask(__name__)
 
 
 
 
def save_to_csv(temperature, humidity):
    with open('data.csv', 'a', newline='') as csvfile:
        fieldnames = ['time','temperature', 'humidity']  
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)  
 
        # Check if the file is empty to write header  
        csvfile.seek(0, 2)  # Move the cursor to the end of the file  
        if csvfile.tell() == 0: # If file is empty, write header
            writer.writeheader()
       
        timezone = pytz.timezone("America/Bogota")
        now = datetime.now(timezone)
        formatted_time = now.strftime("%Y-%m-%d %H:%M:%S")
       
        writer.writerow({'time': formatted_time, 'temperature': temperature, 'humidity': humidity})
 
@app.route('/')
def index():
    return jsonify("IoT server for ESP32")
 
#@app.route('/data', methods=['POST'])
#def data_received():
#    humidity = request.form['humidity']
#    temperature = request.form['temperature']
#    print(f"Humidity: {humidity}%, temperature: {temperature}°C")
#    save_to_csv(temperature,humidity)
 
#    return 'Data received', 200
@app.route('/data', methods=['POST'])
def data_received():
    humidity = request.form.get('humidity')
    temperature = request.form.get('temperature')
 
    if humidity is None or temperature is None:
        return 'Missing data', 400
 
    print(f"Humidity: {humidity}%, temperature: {temperature}°C")
    save_to_csv(temperature, humidity)
    return 'Data received', 200
 
@app.route('/test')
def test():
    return "Hello from Gitpod"
 
if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)