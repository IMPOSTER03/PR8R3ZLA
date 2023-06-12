// JavaScript code for handling form submission and compression
document.getElementById("imageForm").addEventListener("submit", function (event) {
    event.preventDefault();
    var fileInput = document.getElementById("imageFile");
    if (fileInput.files.length === 0) {
        return;
    }
    var file = fileInput.files[0];

    // Perform image compression using appropriate compression algorithm



    // Display the compression result in the result div
    var resultDiv = document.getElementById("result");
    resultDiv.innerHTML = "Image compressed successfully!";
});

document.getElementById("textForm").addEventListener("submit", function (event) {
    event.preventDefault();
    var fileInput = document.getElementById("textFile");
    if (fileInput.files.length === 0) {
        return;
    }
    var file = fileInput.files[0];

    // Perform text compression using appropriate compression algorithm

    // Display the compression result in the result div
    var resultDiv = document.getElementById("result");
    resultDiv.innerHTML = "Text compressed successfully!";
});