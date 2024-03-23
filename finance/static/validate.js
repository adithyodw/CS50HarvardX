const form = document.querySelector('form');
const p = document.getElementById('error');
form.addEventListener('submit', function(event) {
    // Prevent the form from submitting by default
    event.preventDefault();

    // Get the password value
    const passwordValue = document.querySelector('#password').value;

    // Define regular expressions for each password requirement
    const lengthRegex = /.{8,}/; // Minimum length of 8 characters
    const uppercaseRegex = /[A-Z]/; // At least one uppercase letter
    const lowercaseRegex = /[a-z]/; // At least one lowercase letter
    const numberRegex = /\d/; // At least one number
    const symbolRegex = /[^A-Za-z0-9]/; // At least one symbol

    // Check if the password meets all requirements
    if (
        !lengthRegex.test(passwordValue) ||
        !uppercaseRegex.test(passwordValue) ||
        !lowercaseRegex.test(passwordValue) ||
        !numberRegex.test(passwordValue) ||
        !symbolRegex.test(passwordValue)
    ) {
        // Inform user about password requirements
        p.innerText = 'Password must be 8 or more characters long\
        and must contain at least one uppercase letter,\
        one lowercase letter, one number and one symbol\
        i.e("@").';
        return;
    }

    // If the password is valid, let the form submit
    form.submit();
});
