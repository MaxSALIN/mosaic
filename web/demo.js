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

var victoryBool = false;

var offsetx_count = 0;
var offsety_count = 0;


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
    g = Module._new_default();
    drawCanvas(g);
    Module._delete(g);
}



var canvas = document.getElementById('mycanvas');
var currentcolorcanvas = document.getElementById('currentcolor');


/* ******************** global variables ******************** */

var unconstrain = new Image();
unconstrain.src = "res/-1.png";
var empty = new Image();
empty.src = "res/Empty.png";
var black = new Image();
black.src = "res/Black.png";
var white = new Image();
white.src = "res/White.png";

var currentempty = new Image();
currentempty.src = "res/CurrentEmpty.png";
var currentblack = new Image();
currentblack.src = "res/CurrentBlack.png";
var currentwhite = new Image();
currentwhite.src = "res/CurrentWhite.png";
var currenttexte = new Image();
currenttexte.src = "res/CurrentTexte.png";

var victory = new Image();
victory.src = "res/VictoryScreenNew.png";

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
canvas.addEventListener('click', canvasLeftClick);      // left click event
canvas.addEventListener('contextmenu', canvasRightClick);    // right click event 

window.addEventListener("resize", resizeCanvas); // resize event


// buttons event
var solveButton = document.getElementById('solve');
solveButton.addEventListener('click', solver);
var restartButton = document.getElementById('restart');
restartButton.addEventListener('click', restating);

var undo = document.getElementById('undo');
undo.addEventListener('click', undoing);
var redo = document.getElementById('redo');
redo.addEventListener('click', redoing);

var randomButton = document.getElementById('random');
randomButton.addEventListener('click', random);





/* ******************** event callback ******************** */

//function to resize the playable canvas
function resizeCanvas() {

    var newHeight = window.innerWidth;
    var newWidth = window.innerHeight;

    var min;
    if (newHeight>=newWidth){
        min=newWidth;
    }
    else{
        min=newHeight;
    }
    canvas.width = min/2;
    canvas.height = min/2;
    drawCanvas(g);

}

function canvasLeftClick(event) {
    event.preventDefault(); // prevent default context menu to appear...
    // get relative cursor position in canvas
    console.log("left click at position:", event.offsetX, event.offsetY);
    // update position of mario image used by drawCanvas()
    if(victoryBool==false){
        if((((event.offsetY-offsety_count)/(cell_size))>=0) && (((event.offsetX-offsetx_count)/(cell_size))>=0)){
            Module._play_move(g, (event.offsetY-offsety_count)/(cell_size), (event.offsetX-offsetx_count)/(cell_size), currentColor);
        }
        
    }
    drawCanvas(g);
}
function canvasRightClick(event) {
    event.preventDefault(); // prevent default context menu to appear...
    currentColor=(currentColor+1)%3 //changes the current playable color
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
    victoryBool=false;
    drawCanvas(g);
}

function undoing(event){
    event.preventDefault(); // prevent default context menu to appear...
    Module._undo(g);
    drawCanvas(g);
}

function redoing(event){
    event.preventDefault(); // prevent default context menu to appear...
    Module._redo(g);
    drawCanvas(g);
}

function random(event){
    event.preventDefault(); // prevent default context menu to appear...
    var x=Math.floor(Math.random() * 5)+3;
    var y=Math.floor(Math.random() * 5)+3;
    var black_rate=Math.floor(Math.random() * 7)/10+0.2;
    var constraint_rate=Math.floor(Math.random() * 7)/10+0.2;
    g = Module._new_random(x, y, false, FULL, black_rate, constraint_rate);
    drawCanvas(g);
}

/* ******************** canvas drawing ******************** */

function drawCanvas(g) {
    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var height = canvas.height;

    // clear canvas
    ctx.clearRect(0, 0, width, height);

    var current = currentcolorcanvas.getContext('2d');
    var cuurentwidth = currentcolorcanvas.width;
    var currentheight = currentcolorcanvas.height;

    // clear canvas
    current.clearRect(0, 0, cuurentwidth, currentheight);
    if(currentColor==WHITE){
        current.drawImage(currentwhite, 0, 0, cuurentwidth, currentheight);
    }
    if(currentColor==BLACK){
        current.drawImage(currentblack, 0, 0, cuurentwidth, currentheight);
    }
    if(currentColor==EMPTY){
        current.drawImage(currentempty, 0, 0, cuurentwidth, currentheight);
    }
    current.drawImage(currenttexte, 0, 0, cuurentwidth, currentheight);
   

    
  
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



  for(var j = 0; j<Module._nb_cols(g); j++){
        for(var i = 0; i<Module._nb_rows(g); i++){
            
                if(Module._get_color(g, i, j)==EMPTY){
                    ctx.drawImage(empty,j*cell_size+offsetx_count, i*cell_size+offsety_count, cell_size, cell_size);
                }
                else if(Module._get_color(g, i, j)==BLACK){
                    ctx.drawImage(black,j*cell_size+offsetx_count, i*cell_size+offsety_count, cell_size, cell_size);
                }
                else if(Module._get_color(g, i, j)==WHITE){
                    ctx.drawImage(white,j*cell_size+offsetx_count, i*cell_size+offsety_count, cell_size, cell_size);
                }
                else{
                    ctx.drawImage(unconstrain,j*cell_size+offsetx_count, i*cell_size+offsety_count, cell_size, cell_size);
                }
                
                
                
                for(var val=0;val<=9; val++){
                    if(Module._get_constraint(g, i,j)==val){
                        if(Module._get_status(g,i,j)==2){
                            ctx.drawImage(greens[val],j*cell_size+offsetx_count, i*cell_size+offsety_count, cell_size, cell_size);
                        }
                        else if(Module._get_status(g,i,j)==0){
                            ctx.drawImage(reds[val],j*cell_size+offsetx_count, i*cell_size+offsety_count, cell_size, cell_size);
                        }
                        else{
                            ctx.drawImage(blues[val],j*cell_size+offsetx_count, i*cell_size+offsety_count, cell_size, cell_size);
                        }
                        
                    }
                }
                
            }
        }
    
    if(Module._won(g)){
        victoryBool=true;
        ctx.drawImage(victory, 0, 0, width, height);
    }
    
    
    
}
