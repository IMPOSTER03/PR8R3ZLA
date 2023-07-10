<?php
// Assuming you already have a database connection established
$servername = "localhost";
$username = "root:3306";
$password = " ";
$dbname = "myDB";

// Create a connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check the connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

echo "Connected successfully!";


// Rate Limiting
$maxAttempts = 5; // Maximum number of login attempts allowed
$lockoutDuration = 300; // Lockout duration in seconds (e.g., 300 seconds = 5 minutes)

session_start();

// Get the user input from the login form
$email = $_POST['email'];
$password = $_POST['password'];

// Check if the user has reached the maximum number of login attempts
if (isset($_SESSION['login_attempts']) && $_SESSION['login_attempts'] >= $maxAttempts && (time() - $_SESSION['last_login_attempt']) < $lockoutDuration) {
    // User is temporarily locked out
    $timeRemaining = $lockoutDuration - (time() - $_SESSION['last_login_attempt']);
    echo "Too many login attempts. Please try again in $timeRemaining seconds.";
    exit();
}

// Prepare a SQL statement with a placeholder for the email
$sql = "SELECT password_hash FROM Users WHERE email = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("s", $email);
$stmt->execute();
$stmt->bind_result($stored_password_hash);
$stmt->fetch();

// Verify the password
if ($stored_password_hash !== null && password_verify($password, $stored_password_hash)) {
    // User login successful
    // Perform any additional actions or redirect to a different page

    // Reset the login attempts on successful login
    $_SESSION['login_attempts'] = 0;
    $_SESSION['last_login_attempt'] = null;

    echo "Login successful!";
} else {
    // User login failed
    // Increment the login attempts count
    $_SESSION['login_attempts'] = isset($_SESSION['login_attempts']) ? $_SESSION['login_attempts'] + 1 : 1;
    $_SESSION['last_login_attempt'] = time();

    // Handle the error or redirect back to the login page with an error message
    echo "Invalid email or password!";
}

$stmt->close();
$conn->close();
?>
