Module.onRuntimeInitialized = () => { start(); }

const EMPTY = 0;
const WHITE = 1;
const BLACK = 2;
const UNCONSTRAINED = -1;
const FULL = 0;
const ORTHO = 1;
const FULL_EXCLUDE = 2;
const ORTHO_EXCLUDE = 3;

var g = Module._new_default();
var currentColor = WHITE;

var cell_size=0;




let col2str = [" ", "□", "■"];
let num2str = [
    ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"],     // empty
    ["🄋", "➀", "➁", "➂", "➃", "➄", "➅", "➆", "➇", "➈"],  // white
    ["⓿", "❶", "❷", "❸", "❹", "❺", "❻", "❼", "❽", "❾"],     // black
];


function square2str(n, c) {
    if (n == UNCONSTRAINED)
        return col2str[c];
    else
        return num2str[c][n];
}


function printGame(g) {
    var text = "";
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var n = Module._get_constraint(g, row, col);
            var c = Module._get_color(g, row, col);
            var status = Module._get_status(g, row, col);
            text += square2str(n, c);
        }
        text += "\n";
    }

    // put this text in <div> element with ID 'result'
    var elm = document.getElementById('result');
    elm.innerHTML = text;
}

function start() {
    console.log("call start routine");
    
  
    //var g = Module._new_random(6, 6, false, FULL, 0.6, 0.5);
    Module._play_move(g, 0, 1, BLACK);
    //
    printGame(g);
    drawCanvas(g);
    Module._delete(g);
}

var canvas = document.getElementById('mycanvas');

/* ******************** global variables ******************** */

var unconstrain = new Image();
unconstrain.src = "res/-1.png";
var empty = new Image();
empty.src = "res/Empty.png";
var black = new Image();
black.src = "res/Black.png";
var white = new Image();
white.src = "res/White.png";

var blues =[];
var reds =[];
var greens =[];


for(var i=0; i<10; i++){
    var blue = new Image();
    blue.src = "res/Blue/"+ i + "b.png";
    blues.push(blue);
    var red = new Image();
    red.src = "res/Red/"+ i +  "r.png";
    reds.push(red);
    var green = new Image();
    green.src = "res/Green/" + i + "v.png";
    greens.push(green);

}

// initial position in canvas

/* ******************** register events ******************** */

window.addEventListener('load', windowLoad);              // window load
canvas.addEventListener('click', canvasLeftClick);
canvas.addEventListener('contextmenu', canvasRightClick);            // left click event

var solveButton = document.getElementById('solve');
solveButton.addEventListener('click', solver);
var restartButton = document.getElementById('restart');
restartButton.addEventListener('click', restating);

/*var randomButton = document.getElementById('random');
randomButton.addEventListener('click', random);*/


/* ******************** event callback ******************** */

function canvasLeftClick(event) {
    event.preventDefault(); // prevent default context menu to appear...
    // get relative cursor position in canvas
    console.log("left click at position:", event.offsetX, event.offsetY);
    // update position of mario image used by drawCanvas()
    Module._play_move(g, event.offsetX/cell_size, event.offsetY/cell_size, currentColor);
    drawCanvas(g);
}
function canvasRightClick(event) {
    event.preventDefault(); // prevent default context menu to appear...
    currentColor=(currentColor+1)%3
    console.log(currentColor);
    drawCanvas(g);
}

function windowLoad() {
    console.log("window load");
    drawCanvas(g);
}

function solver(event){
    event.preventDefault(); // prevent default context menu to appear...
    Module._solve(g);
    drawCanvas(g);
}

function restating(event){
    event.preventDefault(); // prevent default context menu to appear...
    Module._restart(g);
    drawCanvas(g);
}

/*function random(event){
    event.preventDefault(); // prevent default context menu to appear...
    Module._new_random(g);
    drawCanvas(g);
}*/

/* ******************** canvas drawing ******************** */

function drawCanvas(g) {
    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var height = canvas.height;

    // clear canvas
    ctx.clearRect(0, 0, width, height);

    
var offsetx_count = 0;
  var offsety_count = 0;
  var cell_size1 = width / Module._nb_cols(g);
  var cell_size2 = height /  Module._nb_rows(g);
  if (width >= cell_size2 *  Module._nb_cols(g)) {
    cell_size = cell_size2;
    offsetx_count = (width - Module._nb_cols(g) * cell_size) / 2;
    offsety_count = 0;
  } else if (height > cell_size1 * Module._nb_rows(g)) {
    cell_size = cell_size1;
    offsetx_count = 0;
    offsety_count = (height - Module._nb_rows(g) * cell_size) / 2;
  }



    
    for(var i = 0; i<=4; i++){
        for(var j = 0; j<=4; j++){
            if(Module._get_color(g, i, j)==EMPTY){
                ctx.drawImage(empty, i*cell_size, j*cell_size, cell_size, cell_size);
            }
            else if(Module._get_color(g, i, j)==BLACK){
                ctx.drawImage(black, i*cell_size, j*cell_size, cell_size, cell_size);
            }
            else if(Module._get_color(g, i, j)==WHITE){
                ctx.drawImage(white, i*cell_size, j*cell_size, cell_size, cell_size);
            }
            else{
                ctx.drawImage(unconstrain, i*cell_size, j*cell_size, cell_size, cell_size);
            }
            
            
            
            for(var val=0;val<=9; val++){
                if(Module._get_constraint(g, i,j)==val){
                    if(Module._get_status(g,i,j)==2){
                        ctx.drawImage(greens[val], i*cell_size, j*cell_size, cell_size, cell_size);
                    }
                    else if(Module._get_status(g,i,j)==0){
                        ctx.drawImage(reds[val], i*cell_size, j*cell_size, cell_size, cell_size);
                    }
                    else{
                        ctx.drawImage(blues[val], i*cell_size, j*cell_size, cell_size, cell_size);
                    }
                    
                }
            }
            
        }
    }
    
    
    
}
