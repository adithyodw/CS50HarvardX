import random
from flask import Flask, render_template, request

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/generate_toto', methods=['POST'])
def generate_toto():
    bet_type = request.form['totoBetType']
    numbers = []

    if bet_type == 'ordinary':
        numbers = generate_complex_ai_ordinary_numbers(6)
    elif bet_type.startswith('system'):
        count = int(bet_type[6:])
        numbers = generate_complex_ai_system_numbers(count)
    elif bet_type == 'systemRoll':
        numbers = generate_complex_ai_ordinary_numbers(6) + ['R']

    return render_template('result.html', result=f'AI Suggested Toto Numbers: {", ".join(map(str, numbers))}')

def generate_complex_ai_ordinary_numbers(count):
    return [simulate_complex_random_drawing() for _ in range(count)]

def generate_complex_ai_system_numbers(count):
    numbers = set()
    while len(numbers) < count:
        numbers.add(simulate_complex_random_drawing())
    return list(numbers)

def simulate_complex_random_drawing():
    # Simulate a more complex AI algorithm (replace with your logic)
    return random.randint(1, 49)

if __name__ == '__main__':
    app.run(debug=True)

