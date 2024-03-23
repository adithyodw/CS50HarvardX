from cs50 import get_string


def main():
    # Get user input
    text = get_string("Text: ")

    # Count letters, words, and sentences
    letters = sum(c.isalpha() for c in text)
    words = text.count(" ") + 1
    sentences = text.count(".") + text.count("!") + text.count("?")

    # Calculate L and S
    L = (letters / words) * 100
    S = (sentences / words) * 100

    # Calculate Coleman-Liau index
    index = round(0.0588 * L - 0.296 * S - 15.8)

    # Output the grade level
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


if __name__ == "__main__":
    main()
