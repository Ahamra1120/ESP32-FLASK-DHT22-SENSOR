from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
from datetime import datetime

app = Flask(__name__)
CORS(app)

data_list = []

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/sensor/data', methods=['POST'])
def receive_data():
    temperature = request.form.get('temperature')
    humidity = request.form.get('humidity')

    if temperature is not None and humidity is not None:
        try:
            temperature = float(temperature)
            humidity = float(humidity)
            timestamp = datetime.now().strftime('%d/%m/%Y %H:%M:%S')
            data = {"temperature": temperature, "humidity": humidity, "timestamp": timestamp}
            data_list.append(data)
            return jsonify({"message": "Data received"}), 200
        except ValueError:
            return jsonify({"message": "Invalid data format"}), 400
    else:
        return jsonify({"message": "Missing parameters"}), 400

@app.route('/data', methods=['GET'])
def get_data():
    return jsonify(data_list), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
