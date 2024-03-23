import os
from datetime import datetime, timezone

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    # Start summing holding values
    holding_sum = 0

    # Get stocks type and amount from user
    user_info = db.execute(
        "SELECT symbol, amount FROM amounts WHERE user_id = ?;", user_id
    )

    # Use API to get additional information for each holding
    for holding in user_info:
        req = lookup(holding["symbol"])
        # Store share name and price
        holding["name"] = req["name"]
        holding["price"] = req["price"]

        # Calculate holding value
        holding["value"] = holding["price"] * holding["amount"]

        # Update total holdings value
        holding_sum += holding["value"]

    # Get the user cash info
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?;", user_id)[0].get(
        "cash"
    )

    # Calculate total user's net worth
    net_worth = holding_sum + user_cash

    return render_template(
        "index.html", user_info=user_info, user_cash=user_cash, net_worth=net_worth
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Check both symbol and shares were provided
        try:
            symbol = request.form.get("symbol")
            if not symbol:
                raise ValueError
            shares = int(request.form.get("shares"))
        except:
            return apology("Provide a valid stock and number", 400)

        # Check if shares is positive integer
        if shares < 1:
            return apology("Shares must be 1 or more", 400)

        # Take result from endpoint and check
        result = lookup(symbol)
        if not result:
            return apology("Could not find symbol", 400)

        # Prepare data for process info
        user_id = session["user_id"]
        d = datetime.now(timezone.utc)
        date = d.strftime("%Y-%m-%d %H:%M:%S")

        # Check user's balance
        total = result["price"] * shares
        q = db.execute("SELECT cash FROM users WHERE id = ?;", user_id)
        balance = q[0]["cash"]

        # If not enough funds, return
        if balance < total:
            return apology("Insufficient funds", 402)

        # Deduce amount from user's balance
        balance -= total
        db.execute("UPDATE users SET cash = ? WHERE id = ?;", balance, user_id)

        # Save new transaction
        db.execute(
            """
                    INSERT INTO history (user_id, symbol, price, shares, type, date)
                    VALUES (?, ?, ?, ?, ?, ?);
                    """,
            user_id,
            symbol,
            result["price"],
            shares,
            "buy",
            date,
        )

        # Add purchase to amounts table (or ignore if already exists)
        db.execute(
            "INSERT OR IGNORE INTO amounts (user_id, symbol, amount) VALUES (?, ?, ?)",
            user_id,
            symbol,
            0,
        )
        db.execute(
            "UPDATE OR IGNORE amounts SET amount = amount + ? WHERE user_id = ? AND symbol = ?",
            shares,
            user_id,
            symbol,
        )

        return redirect("/")

    # Accessed via GET
    else:
        return render_template("pages/buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]

    # Get all transactions history from user
    history = db.execute("SELECT * FROM history WHERE user_id = ?", user_id)
    return render_template("pages/history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("auth/login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # Check if user provided a symbol
        symbol = request.form.get("symbol")
        if symbol:
            quote = lookup(symbol)
            # Check if provided symbol was found in endpoint
            return render_template("pages/quoted.html", quote=quote, symbol=symbol)
        else:
            return apology("must provide symbol", 400)

    # Accessed via GET
    else:
        return render_template("pages/quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)

        # Store username and password
        username = request.form.get("username")
        password = request.form.get("password")

        # Check for secure password
        # if not validate_password(password):
        #     return apology("Must provide stronger password", 400)

        # Ensure passwords match
        if not password == request.form.get("confirmation"):
            return apology("passwords must match", 400)

        # Hash password before inserting to DB
        hashed_pass = generate_password_hash(password)

        # Create user if not already registered
        try:
            db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?);",
                username,
                hashed_pass,
            )
            return redirect("/login")
        except:
            return apology("Username already used", 400)

    # Accessed via GET
    else:
        return render_template("auth/register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]

    if request.method == "GET":
        """Sell shares of stock"""

        # Query for input current stocks
        stocks = db.execute("SELECT symbol FROM amounts WHERE user_id = ?;", user_id)
        return render_template("pages/sell.html", stocks=stocks)

    # User accessed via POST
    else:
        # Verify user provided symbol and shares
        try:
            symbol = request.form.get("symbol")
            if not symbol:
                raise ValueError
            shares = int(request.form.get("shares"))
        except:
            return apology("Provide a valid stock and number", 400)

        # Verify shares is a positive number
        if shares < 1:
            return apology("Shares must be 1 or more", 400)

        # Query database to get the user current holdings state
        q = db.execute(
            """SELECT amount FROM amounts
                       WHERE user_id = ?
                       AND symbol = ?;""",
            user_id,
            symbol,
        )

        # Verify db results
        if not len(q) == 1:
            return apology("Something went wrong", 400)

        # Get the user share current amount
        current_shares = q[0].get("amount")
        current_shares -= shares

        # If user does not have enough shares, apologize
        if current_shares < 0:
            return apology("You do not own that many shares", 400)
        else:
            # Save date
            d = datetime.now(timezone.utc)
            date = d.strftime("%Y-%m-%d %H:%M:%S")

            # Request endpoint for share price
            price = lookup(symbol).get("price")

            # Save new transaction
            db.execute(
                """
                       INSERT INTO history (user_id, symbol, price, shares, type, date)
                       VALUES (?, ?, ?, ?, ?, ?);
                       """,
                user_id,
                symbol,
                price,
                shares,
                "sell",
                date,
            )

            # Update cash from user
            db.execute(
                """UPDATE users
                       SET cash = cash + ?
                       WHERE id = ?;""",
                (price * shares),
                user_id,
            )

            # User sold the whole holding
            if current_shares == 0:
                db.execute(
                    """DELETE FROM amounts
                           WHERE user_id = ?
                           AND symbol = ?;""",
                    user_id,
                    symbol,
                )
            # User sold a portion of the holding
            else:
                db.execute(
                    """UPDATE amounts
                           SET amount = amount - ?
                           WHERE user_id = ?
                           AND symbol = ?;""",
                    shares,
                    user_id,
                    symbol,
                )

            return redirect("/")


@app.route("/change-password", methods=["GET", "POST"])
def change_password():
    """Let user change password"""
    # User reached route via POST
    if request.method == "POST":
        user_id = session["user_id"]
        # Validate current password
        if not request.form.get("old-password"):
            return apology("must provide password", 400)

        # Query database for user data
        rows = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("old-password")
        ):
            return apology("invalid password", 403)

        # Ensure new password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)

        # Store new password
        password = request.form.get("password")

        # # Check for secure password
        # if not validate_password(password):
        #     return apology("Must provide stronger password", 400)

        # Ensure passwords match
        if not password == request.form.get("confirmation"):
            return apology("passwords must match", 400)

        # Hash password before inserting to DB
        hashed_pass = generate_password_hash(request.form.get("password"))

        # Create user
        try:
            db.execute("UPDATE users SET hash = ? WHERE id = ?;", hashed_pass, user_id)
            return redirect("/")
        except:
            return apology("Something went wrong", 400)

    # User accessed via GET
    else:
        return render_template("auth/change-password.html")
