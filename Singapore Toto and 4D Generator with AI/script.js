function generateTotoNumbers() {
  const betType = document.getElementById('totoBetType').value;
  let numbers;

  switch (betType) {
    case 'ordinary':
      numbers = generateComplexAIOrdinaryNumbers();
      break;
    case 'system7':
    case 'system8':
    case 'system9':
    case 'system10':
    case 'system11':
    case 'system12':
      numbers = generateComplexAISystemNumbers(parseInt(betType.substring(6)));
      break;
    case 'systemRoll':
      numbers = generateComplexAIOrdinaryNumbers().concat('R');
      break;
    default:
      numbers = generateComplexAIOrdinaryNumbers();
  }

  displayResult('totoResult', `AI Suggested Toto Numbers: ${numbers.join(', ')}`);
}

function generateComplexAIOrdinaryNumbers() {
  const numbers = [];
  for (let i = 0; i < 6; i++) {
    numbers.push(simulateComplexRandomDrawing());
  }
  return numbers;
}

function generateComplexAISystemNumbers(count) {
  const numbers = new Set();
  while (numbers.size < count) {
    numbers.add(simulateComplexRandomDrawing());
  }
  return Array.from(numbers);
}

function simulateComplexRandomDrawing() {
  return Math.floor(Math.random() * 49) + 1;
}

function generate4DNumbers() {
  const betType = document.getElementById('4dBetType').value;
  let numbers;

  switch (betType) {
    case 'ordinaryEntry':
      numbers = generate4DOrdinaryEntry();
      break;
    case 'roll':
      numbers = generate4DRoll();
      break;
    case 'systemEntry':
      numbers = generate4DSystemEntry();
      break;
    default:
      numbers = generate4DOrdinaryEntry();
  }

  displayResult('4dResult', `AI Suggested 4D Numbers: ${numbers.join('')}`);
}

function generate4DOrdinaryEntry() {
  const numbers = [];
  for (let i = 0; i < 4; i++) {
    numbers.push(Math.floor(Math.random() * 10));
  }
  return numbers;
}

function generate4DRoll() {
  const numbers = [];
  for (let i = 0; i < 3; i++) {
    numbers.push(Math.floor(Math.random() * 10));
  }
  numbers.push('R');
  return numbers;
}

function generate4DSystemEntry() {
  const numbers = [];
  for (let i = 0; i < 4; i++) {
    numbers.push(Math.floor(Math.random() * 10));
  }
  return numbers;
}

function displayResult(resultId, result) {
  const resultDiv = document.getElementById(resultId);
  resultDiv.innerHTML = `<p>${result}</p>`;
}
