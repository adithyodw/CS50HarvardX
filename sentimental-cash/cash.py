from cs50 import get_float


def main():
    while True:
        change_owed = get_float("Change owed: ")
        if change_owed >= 0:
            break

    cents = round(change_owed * 100)
    coins = 0

    # Calculate the number of coins needed
    coins += cents // 25  # quarters
    cents %= 25

    coins += cents // 10  # dimes
    cents %= 10

    coins += cents // 5  # nickels
    cents %= 5

    coins += cents  # pennies

    print(coins)


if __name__ == "__main__":
    main()
