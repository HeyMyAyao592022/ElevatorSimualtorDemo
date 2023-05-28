Qt.include("three.js")
Qt.include("tween.js")
Qt.include("tween.min.js")
Qt.include("ThreeJSLoader.js")
Qt.include("OrbitControls.js")

const colorBackground = 0x292D3E;
const colorPrimary = 0x80CBC4;
// floor
const colorFloor1 = 0xF5FFFA
const colorFloor2 = 0xF2FDEF
const colorFloor3 = 0xFFFAFA
const colorFloor4 = 0xFDEFEF
// el
const elColors = [0xF5EEF8, 0xEBF5FB, 0xFDF2E9, 0xD6EAF8, 0xEAFAF1]
const elTypes = [0,0,1,1,2,2,3,3,4,4]
// el
const colorEl = 0xc0c0c0;
const colorGround = 0x292D3E;
const colorGolden = 0xFfd700
const colorPassenger = 0xFF7F50

// canva
let qmlCanvas = null
// 相机
let camera = null
const nearDistance = 210
// 场景
let scene = null
// 渲染器
let renderer = null
// font
let textFont = null
let text_material = null
// timer
let qmlTimer = null
let preConnFun = null
// canva
let textCanvas = null
// 控制器
let controls = null

// 楼层
const floorHeight = 12
const floorWidth = 20
const floorLength = 160
const floorThick = 1

// 电梯
const elWidth = 3
const elHeight = 7

// data
let elNum = 10
let elSpeed = 2000
let floorNum = 40

// position
const floorPosY = [] // 1~40对应
const elPosX = -elWidth/2 - 1
const elPosY = new Array
const elPosZ = new Array

// el
const elevators = [] // 0~9对应

// camera viewModel
let cameraModel = 2
let cameraModelIndex = 0

// 乘客
const passengerMap = new Map()
let passengerObj

// tweens
const elStateTweens = new Array
const elUpdatePassengerTweens = new Array

function initializeGL(canvas)
{
    // canvas
    qmlCanvas = canvas

    // 设置场景
    scene = new THREE.Scene();

    // 设置相机相关参数
    camera = new THREE.PerspectiveCamera(45, canvas.width / canvas.height, 0.1, 1000);

    // 添加电梯和楼层
    scene.add(iniFloors())
    iniWall()
    iniEls(scene)

    // 添加地板
    iniGround()

    // 将相机位置和视点放在场景中间
    camera.position.x = nearDistance;
    camera.position.y = floorPosY[5];
    camera.position.z = 0;
    camera.lookAt({x:0,y:floorPosY[5],z:0});

    // light
    iniLight()

    // timer
    createTimer()

    // text
    iniText()

    // 创建一个渲染器
    renderer = new THREE.Canvas3DRenderer(
                { canvas: canvas, antialias: true, devicePixelRatio: canvas.devicePixelRatio ,alpha:true});
    renderer.setSize(canvas.width, canvas.height);
    // 设置画布的背景颜色
    renderer.setClearColor(colorBackground,1.0);

    // 初始化乘客材质
    passengerObj = new THREE.Mesh(new THREE.CylinderGeometry( 2, 2, 5, 8 ),
                                  new THREE.MeshBasicMaterial({color: colorPassenger}))
    passengerObj.position.x = 5
}

function resizeGL(canvas)
{

    camera.aspect = canvas.width / canvas.height;
    camera.updateProjectionMatrix();

    renderer.setPixelRatio(canvas.devicePixelRatio);
    renderer.setSize(canvas.width, canvas.height);
}

function paintGL(canvas)
{
    TWEEN.update()
    //controls.update()
    renderer.render(scene, camera);
}

/// 交互

// 设置初始值
const iniView = (elNumVal, elSpeedVal)=>{
    elNum = elNumVal
    elSpeed = elSpeedVal
}

// 生成乘客
const generateNewPassenger = (id, from, to)=>{
    // console.log("in js: adding psg " + id +" in floor " + from)
    // 创建乘客
    const newPassenger = passengerObj.clone()
    // 插入 map
    passengerMap.set(id,newPassenger)
    // 设置位置
    newPassenger.position.y = floorPosY[from] + 2.5
    scene.add(newPassenger)
}

// 绑定、载客、卸客

const connectPassenger = (passengerID, elID, consumeTime)=>{
    const psg = passengerMap.get(passengerID)
    const posz = elPosZ[elID]
    // 根据距离确定时间
    let durationTime = 1500
    if(elNum >= 7){
        if(elID <= 2 || elID >= 6){
            durationTime = 3000
        }
    }
    // z
    const tween =  new TWEEN.Tween(psg.position)
    tween.to({z:posz},durationTime)
    // link
    tween.start()
}

const loadPassenger = (passengerID, elID, consumeTime)=>{
    const psg = passengerMap.get(passengerID)
    const durationTime = consumeTime*1000
    // x
    const tween =  new TWEEN.Tween(psg.position)
    tween.to({x:elPosX},durationTime)
    tween.onComplete(function(){
        scene.remove(psg)
    })
    // link
    tween.start()
}

const unLoadPassenger = (elID, psgID, consumeTime)=>{
    // psg
    const psg = passengerMap.get(psgID)
    psg.position.y = elevators[elID].position.y
    scene.add(psg)
    // time
    const time = consumeTime*1000 - 800
    // x
    const tween1 =  new TWEEN.Tween(psg.position)
    tween1.to({x:12},time)
    // z
    let posz = 0
    if(elID < elNum/2){
        posz = psg.position.z - 10
    }
    else{
        posz = psg.position.z + 10
    }
    const tween2 =  new TWEEN.Tween(psg.position)
    tween2.to({z:posz},600)
    tween2.onComplete(function(){
        scene.remove(psg)
    })
    // link
    tween1.chain(tween2)
    tween1.start()
}

// 电梯移动
const updateElState = (target,newFloor)=>{
    const twn = elStateTweens[target]
    twn.to({y:elPosY[newFloor]},elSpeed)
    twn.start()
}

const updataCamera = (model, index)=>{
    // 提前声明变量
    let posx
    let posy
    let tweenX = null
    let tweenY = null
    const duration = 1800
    /// 切换监控视角
    if(model === 2){
        // 未变动就无需执行
        if(cameraModel === model && cameraModelIndex === index){
            return;
        }
        // 设置 x
        posx = nearDistance;
        // 设置 y
        switch(index){
            case 0:
            posy = floorPosY[5]
            break;
            case 1:
            posy = floorPosY[15]
            break;
            case 2:
            posy = floorPosY[25]
            break;
            case 3:
            posy = floorPosY[35]
            break;
        }
        // 设置 tween y
        tweenY = new TWEEN.Tween(camera.position)
        .to({y:posy},duration)
        .easing(TWEEN.Easing.Sinusoidal.InOut)
        // 设置 tween x
        if(model !== cameraModel){
            tweenX = new TWEEN.Tween(camera.position)
            .to({x:posx},duration)
            .easing(TWEEN.Easing.Sinusoidal.InOut)
        }
        // 连接并启动动画
        if(tweenX !== null){
            tweenY.start()
            tweenX.start()
        }
        else{
            tweenY.start()
        }
    }
    // 远景视角
    else if(model === 0){
        // 未变动就无需执行
        if(cameraModel === model){
            return;
        }
        // 设置 x
        posx = nearDistance + 650
        tweenX = new TWEEN.Tween(camera.position)
        .to({x:posx},duration)
        .easing(TWEEN.Easing.Sinusoidal.InOut)
        // 设置 y
        posy = floorPosY[15]
        tweenY = new TWEEN.Tween(camera.position)
        .to({y:posy},duration)
        .easing(TWEEN.Easing.Sinusoidal.InOut)
        // 连接并启动
        tweenY.start()
        tweenX.start()
    }
    else if(model === 1){
        // 设置 x
        posx = nearDistance;
        // 设置 y
        posy = elevators[index].position.y
        // 设置 tween y
        tweenY = new TWEEN.Tween(camera.position)
        .to({y:posy},duration)
        .easing(TWEEN.Easing.Sinusoidal.InOut)
        // 设置 tween x
        if(model !== cameraModel){
            tweenX = new TWEEN.Tween(camera.position)
            .to({x:posx},duration)
            .easing(TWEEN.Easing.Sinusoidal.InOut)
        }
        // 连接并启动动画
        if(tweenX !== null){
            tweenY.start()
            tweenX.start()
        }
        else{
            tweenY.start()
        }
    }
    // 保存值
    cameraModel = model
    cameraModelIndex = index
}


/// 电梯
// 在 initFloorPos 之后
const iniSingleElPosY = (floor)=>{
    return floorPosY[floor] + 0.5 + elHeight/2
}

function iniElPos(){
    elPosY.push(0)
    for(let i = 1; i <= floorNum; i++){
        elPosY.push(iniSingleElPosY(i))
    }
    // 创建 z 坐标
    const zMargin = 15
    const zOffSet = (elNum/2 - 1)*zMargin + 7
    for(let j = 0; j < elNum; j++){
        elPosZ.push(zOffSet - zMargin*j)
    }
}

const iniEls = (view)=>{
    iniElPos()
    // geometry
    const geometry = new THREE.BoxGeometry(elWidth,elHeight,elWidth)
    // material
    const materials = new Array
    for(let j = 0; j < 5; j++){
        materials.push(new THREE.MeshBasicMaterial({color: elColors[j]}))
    }
    for(let i = 0; i < elNum; i++){
        const el = new THREE.Mesh(geometry, materials[elTypes[i] % 5])
        el.position.x = elPosX
        el.position.y = elPosY[1] // 初始位置都是 1 楼
        el.position.z = elPosZ[i]
        view.add(el) // 添加到场景中
        elevators.push(el) // 添加到数组中
        // elPosState.push(1) // 初始位置都是 1 楼
        // 添加 tween
        elStateTweens.push(new TWEEN.Tween(elevators[i].position))
    }
}

/// 楼梯
const iniFloorPos = ()=>{
    floorPosY.push(0)
    for(let i = 0; i < floorNum; i++)
    {
        floorPosY.push(i*floorHeight)
    }
}

const iniFloors = ()=>{

    iniFloorPos()

    const floors = new THREE.Group()
    const floorGeometry = new THREE.BoxGeometry(floorWidth,floorThick,floorLength)
    const floorMaterial1 = new THREE.MeshBasicMaterial({color: colorFloor1,transparent: true,opacity:0.6})
    const floorMaterial2 = new THREE.MeshBasicMaterial({color: colorFloor2,transparent: true,opacity:0.6})
    const floorMaterial3 = new THREE.MeshBasicMaterial({color: colorFloor3,transparent: true,opacity:0.8})
    const floorMaterial4 = new THREE.MeshBasicMaterial({color: colorFloor4,transparent: true,opacity:0.8})
    for(let i = 1; i <= floorNum; i++)
    {
        let floor = null
        if(i %5 === 0){
            if(i % 10 === 0){
                floor = new THREE.Mesh(floorGeometry,floorMaterial4)
            }
            else{
                floor = new THREE.Mesh(floorGeometry,floorMaterial3)
            }
        }
        else if(i < 25){
            floor = new THREE.Mesh(floorGeometry,floorMaterial1)
        }
        else{
            floor = new THREE.Mesh(floorGeometry,floorMaterial2)
        }
        floor.position.x = floorWidth/2
        floor.position.y = floorPosY[i]
        floors.add(floor)
    }

    return floors
}

const iniWall = ()=>{
    const width = floorWidth + elWidth + 10
    const height = floorHeight*floorNum

    const floorGeometry = new THREE.BoxGeometry(floorWidth,floorThick,floorLength)
    const floorMaterial1 = new THREE.MeshBasicMaterial({color: colorFloor1,transparent: true,opacity:0.6})
    const floorMaterial2 = new THREE.MeshBasicMaterial({color: colorFloor1,transparent: true,opacity:0.2})

    const top =  new THREE.Mesh(floorGeometry,floorMaterial1)
    top.position.x = floorWidth/2
    top.position.y = floorPosY[floorNum] + floorHeight
    scene.add(top)

    const wall1 = new THREE.Mesh(new THREE.BoxGeometry(width,floorThick,height)
                                 ,floorMaterial1)
    wall1.rotation.x = Math.PI/2
    wall1.position.z = floorLength/2 + 1
    wall1.position.y = height/2

    const wall2 = wall1.clone()
    wall2.position.z = -floorLength/2 - 1

    scene.add(wall1)
    scene.add(wall2)

    const wall3 = new THREE.Mesh(new THREE.BoxGeometry(floorLength,floorThick,height)
                                 ,floorMaterial2)
    wall3.position.x = -width/2
    wall3.position.y = height/2
    wall3.rotation.x = Math.PI/2
    wall3.rotation.z = Math.PI/2

    scene.add(wall3)
}

/// ground
const iniGround = ()=>{
    const grid = new THREE.GridHelper( 300, 20, 0x000000, 0x000000 );
    grid.material.opacity = 0.5;
    grid.material.transparent = true;
    scene.add( grid );
}

/// light
const iniLight = ()=>{
//    const ambient = new THREE.AmbientLight(0xffffff, 0.7) //环境光
//    ambient.position.set(200, 300, 50) //点光源位置
//    scene.add(ambient)

//    const directionalLight = new THREE.DirectionalLight(0x9B59B6, 1) //方向光
//    directionalLight.position.set(1, 20, 1)
//    scene.add(directionalLight)
}

/// text
const iniText = ()=>{
    //    const loader=new THREE.FontLoader()
    //    text_material =new THREE.MeshBasicMaterial({color:0xffffff});
    //    loader.load("qrc:///consolas.typeface.js", function(font){
    //        // 设置 font
    //        textFont = font
    //        createText(10,"F10",text_material)
    //        createText(20,"F20",text_material)
    //        createText(25,"F25",text_material)
    //        createText(30,"F30",text_material)
    //        createText(40,"F40",text_material)
    //    });
}

//const iniText = ()=>{
//    textCanvas = Qt.createQmlObject("import QtQuick 2.12; Canvas { }", qmlCanvas);
//    textCanvas.onAvailableChanged.connect(function(){
//        drawCanvas()
//    })
//}

const drawCanvas = () =>{
    if(textCanvas.available === true){
        let ctx = textCanvas.getContext("2d")
        ctx.fillStyle = "rgb(255,255,250)"
        textCanvas.requestPaint()
        ctx.fillText("35", 0, 0)
        const texture = new THREE.Texture(textCanvas);
        texture.needsUpdate = true;
        //texture.wrapS = THREE.RepeatWrapping;
        //texture.wrapT = THREE.RepeatWrapping;
        //创建精灵，将该材质赋予给创建的精灵
        var material = new THREE.SpriteMaterial({
                                                    //map: texture,
                                                });
        var particle = new THREE.Sprite(material);
        //particle.scale.set(25, 8, 10);
        //particle.position.set(0, 2, 80)
        //scene.add(particle);
    }
}

const createText = (floor, str, textMaterial)=>{
    var new_text=new THREE.TextGeometry(str, {
                                            font:textFont,
                                            size:3,
                                            height:0.1,
                                        });
    var text=new THREE.Mesh(new_text, textMaterial);
    //创建文字
    scene.add(text);
    //添加文字
    text.position.z= 80;
    text.position.y= 2 + floorPosY[floor];
    text.position.x= 0;
    text.rotation.y = Math.PI/2
}

/// time out
function createTimer(){
    qmlTimer = Qt.createQmlObject("import QtQuick 2.12; Timer {}", qmlCanvas);
    qmlTimer.repeat = false;
}

const setTimeout = (callback,timeout) =>{
    if(preConnFun !== null){
        qmlTimer.triggered.disconnect(preConnFun)
    }
    preConnFun = callback
    qmlTimer.interval = timeout;
    qmlTimer.triggered.connect(preConnFun)
    qmlTimer.start();
}


