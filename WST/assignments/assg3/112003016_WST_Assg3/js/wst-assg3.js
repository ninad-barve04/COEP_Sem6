
var imagesList = {
    wildlife: [
        "./images/image1.jpg",
        "./images/image2.jpg",
        "./images/image3.jpg",
        "./images/image4.jpg",
        "./images/image5.jpg"
    ],
    monuments: [
        "./images/m-image1.jpg",
        "./images/m-image2.jpg",
        "./images/m-image3.jpg",
        "./images/m-image4.jpg",
        "./images/m-image5.jpg"
    ],
    forts: [
        "./images/f-image1.jpg",
        "./images/f-image2.jpg",
        "./images/f-image3.jpg",
        "./images/f-image4.jpg",
        "./images/f-image5.jpg"
    ]
}

var images = imagesList['wildlife'];

var imageindex = 0;
var timedelay = 5;
var autotimer;

function initialize() {

    imageindex = 0;
    timedelay = 5;
    autotimer = setInterval(nextImage, timedelay * 1000);
    images = imagesList['wildlife'];
    console.log(images);
}

function nextImage() {
    if (images) {
        imageindex = imageindex + 1;
        if (imageindex == images.length) {
            imageindex = 0;
        }
    }

    document.getElementById("imgplace").src = images[imageindex];
}

function prevImage() {
    imageindex = imageindex - 1;
    if (imageindex < 0) {
        imageindex = images.length - 1;
    }

    document.getElementById("imgplace").src = images[imageindex];
}


function setTimeDelay(value) {
    timedelay = value;
    clearInterval(autotimer);
    autotimer = setInterval(nextImage, timedelay * 1000);
}

function setAutoScroll(checked) {
    if (checked) {
        autotimer = setInterval(nextImage, timedelay * 1000);
    }
    else {
        clearInterval(autotimer);
    }
}

function setGallery(galary) {
    images = imagesList[galary];
    imageindex = 0;
    document.getElementById("imgplace").src = images[imageindex];
}
