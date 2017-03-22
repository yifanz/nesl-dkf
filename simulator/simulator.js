if ( ! Detector.webgl ) Detector.addGetWebGLMessage();

var stats;
var camera, controls, scene, renderer;
var textLabels = [];

init();
// remove when using next line for animation loop (requestAnimationFrame)
render();
// animate();

function init() {

    scene = new THREE.Scene();
    //scene.fog = new THREE.FogExp2( 0xcccccc, 0.002 );

    camera = new THREE.PerspectiveCamera( 60, window.innerWidth / window.innerHeight, 1, 5000 );
    camera.position.z = 8;
    camera.position.y = 6;
    camera.position.x = 0;
    camera.lookAt(new THREE.Vector3(0,0,0));

    renderer = new THREE.WebGLRenderer({antialias: true});
    renderer.setClearColor( 0xcccccc );
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight );

    renderer.shadowMapEnabled = true;
    renderer.shadowMapSoft = false;

    renderer.shadowCameraNear = 50;
    renderer.shadowCameraFar = camera.far;
    renderer.shadowCameraFov = 60;

    renderer.shadowMapBias = 0.0039;
    renderer.shadowMapDarkness = 0.5;
    renderer.shadowMapWidth = 1024;
    renderer.shadowMapHeight = 1024;

    var container = document.getElementById( 'container' );
    container.appendChild( renderer.domElement );

    controls = new THREE.OrbitControls( camera, renderer.domElement );
    controls.addEventListener( 'change', render ); // remove when using animation loop
    // enable animation loop when using damping or autorotation
    //controls.enableDamping = true;
    //controls.dampingFactor = 0.25;
    controls.enableZoom = true;

    // world

    //var geometry = new THREE.CylinderGeometry( 0, 10, 30, 4, 1 );
    var geometry = new THREE.BoxGeometry(0.2, 0.2, 0.2);
    var material =  new THREE.MeshPhongMaterial(
            { color:0x2194ce, shading: THREE.SmoothShading }
            );

    var gridHelper = new THREE.GridHelper( 20, 40, 0x0000ff, 0x808080 );
    gridHelper.position.y = 0;
    gridHelper.position.x = 0;
    scene.add( gridHelper );

    var floor_geo = new THREE.PlaneGeometry( 20, 20 );
    var floor_material = new THREE.MeshPhongMaterial(
            {color: 0x8c8c8c, side: THREE.DoubleSide} );
    var plane = new THREE.Mesh( floor_geo, floor_material );
    plane.rotateX(Math.PI / 2);
    plane.position.y -= 0.1;
    plane.receiveShadow = true;
    scene.add( plane );

    var group = new THREE.Object3D();
    var anchor_names = [
        "ntb_alpha",
        "ntb_bravo",
        "ntb_charlie",
        "ntb_delta",
        "ntb_echo",
        "ntb_foxtrot",
        "ntb_golf",
        "ntb_hotel",
        ];
    var anchors = [];
    for (var i = 0; i < 8; i++) {
        var mesh = new THREE.Mesh( geometry, material );
        mesh.position.x = DATA[i][0];
        mesh.position.y = DATA[i][1];
        mesh.position.z = DATA[i][2];
        mesh.updateMatrix();
        mesh.matrixAutoUpdate = false;
        mesh.castShadow = true;
        group.add( mesh );
        anchors.push(mesh);

        var label = createLabel();
        label.setHTML(anchor_names[i]);
        label.setParent(mesh);
        textLabels.push(label);
        container.appendChild(label.element);
    }

    var line_material = new THREE.LineBasicMaterial({
        color: 0xffe6a5
    });
    var lines = [];

    var tag_material =  new THREE.MeshPhongMaterial(
            { color:0xffdc41, shading: THREE.SmoothShading }
            );
    var tags = [];
    var counter = 16;
    for (var i = 8; i < 16; i++) {
        var tag = new THREE.Mesh( geometry, tag_material );
        tag.position.x = DATA[i][0];
        tag.position.y = DATA[i][1];
        tag.position.z = DATA[i][2];
        tag.castShadow = true;
        group.add(tag);
        tags.push(tag);

        var line_geo = new THREE.Geometry();
        line_geo.vertices.push(
                new THREE.Vector3( tag.position.x, tag.position.y, tag.position.z ),
                new THREE.Vector3(
                    anchors[i-8].position.x,
                    anchors[i-8].position.y,
                    anchors[i-8].position.z
                    )
                );
        var range_line = new THREE.Line( line_geo, line_material );
        group.add(range_line);
        lines.push(range_line);
    };

    group.translateX(-DATA[4][0]);
    //group.translateY();
    //group.translateZ();
    scene.add(group);

    function updateTag() {
        for (var i = 0; i < 8; i++) {
            var tag = tags[i];
            tag.position.x = DATA[counter+i][0];
            tag.position.y = DATA[counter+i][1];
            tag.position.z = DATA[counter+i][2];

            group.remove(lines[i]);
            var line_geo = new THREE.Geometry();
            line_geo.vertices.push(
                    new THREE.Vector3( tag.position.x, tag.position.y, tag.position.z ),
                    new THREE.Vector3(
                        anchors[i].position.x,
                        anchors[i].position.y,
                        anchors[i].position.z
                        )
                    );
            var range_line = new THREE.Line( line_geo, line_material );
            group.add(range_line);
            lines[i] = range_line;
        };
        counter += 8;
        render();
        if (counter < DATA.length)
            setTimeout(updateTag, 100);
    }


    // lights
    var directionalLight = new THREE.DirectionalLight( 0xffffff, 0.5 );
    directionalLight.castShadow = true;
    directionalLight.position.set(0,5,0);
    directionalLight.shadowCameraLeft = -6;
    directionalLight.shadowCameraRight = 6;
    directionalLight.shadowCameraTop = 6;
    directionalLight.shadowCameraBottom = -6;
    scene.add(directionalLight);
    /*
       light = new THREE.DirectionalLight( 0xffffff );
       light.position.set( 1, 1, 1 );
       scene.add( light );

       light = new THREE.DirectionalLight( 0x002288 );
       light.position.set( -1, -1, -1 );
       scene.add( light );
       */

    light = new THREE.AmbientLight( 0xffffff );
    scene.add( light );

    stats = new Stats();
    //container.appendChild( stats.dom );

    window.addEventListener( 'resize', onWindowResize, false );

    setTimeout(updateTag, 100);
}

function onWindowResize() {

    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();

    renderer.setSize( window.innerWidth, window.innerHeight );

}

function animate() {

    requestAnimationFrame( animate );

    controls.update(); // required if controls.enableDamping = true, or if controls.autoRotate = true

    stats.update();

    render();

}

function render() {

    for (var i = 0; i < textLabels.length; i++) {
        textLabels[i].updatePosition();
    }
    renderer.render( scene, camera );

}

function createLabel() {
    var div = document.createElement('div');
    div.className = 'text-label';
    div.style.position = 'absolute';
    div.style.width = 100;
    div.style.height = 100;
    div.innerHTML = "hi there!";
    div.style.top = -1000;
    div.style.left = -1000;

    var _this = this;

    return {
      element: div,
      parent: false,
      position: new THREE.Vector3(0,0,0),
      setHTML: function(html) {
        this.element.innerHTML = html;
      },
      setParent: function(threejsobj) {
        this.parent = threejsobj;
      },
      updatePosition: function() {
        if(parent) {
          this.position.copy(this.parent.position);
        }

        this.position.x += -DATA[4][0];
        var coords2d = this.get2DCoords(this.position, camera);
        this.element.style.left = coords2d.x + 'px';
        this.element.style.top = coords2d.y + 'px';
      },
      get2DCoords: function(position, camera) {
        var vector = position.project(camera);
        vector.x = (vector.x + 1)/2 * window.innerWidth;
        vector.y = -(vector.y - 1)/2 * window.innerHeight;
        return vector;
      }
    };
  }
