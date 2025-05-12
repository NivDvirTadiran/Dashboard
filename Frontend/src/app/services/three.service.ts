import { Injectable, ElementRef } from '@angular/core';
import * as THREE from 'three';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader.js';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls.js';
import { EdgeSplitModifier } from 'three/examples/jsm/modifiers/EdgeSplitModifier.js';
import {BufferGeometry} from "three";
//import { FontLoader } from 'three/examples/jsm/loaders/FontLoader';
//import { TextGeometry } from 'three/examples/jsm/geometries/TextGeometry';

@Injectable({
  providedIn: 'root'
})
export class ThreeService {
  private scene: THREE.Scene;
  private camera: THREE.PerspectiveCamera;
  private renderer: THREE.WebGLRenderer;
  private model: THREE.Group | undefined;
  private controls: OrbitControls | undefined;
  private directionalLight: THREE.DirectionalLight;
  private lightDirection: any;
  private arrowHelperX: THREE.ArrowHelper | undefined;
  private arrowHelperY: THREE.ArrowHelper | undefined;
  private arrowHelperZ: THREE.ArrowHelper | undefined;
  private textMesh: THREE.Mesh | undefined;
  private floor: THREE.Mesh | undefined;

  constructor() {
    this.scene = new THREE.Scene();
    this.camera = new THREE.PerspectiveCamera(50, window.innerWidth / window.innerHeight, 0.001, 10000);
    this.renderer = new THREE.WebGLRenderer({ alpha: true, antialias: true, logarithmicDepthBuffer: true, powerPreference: 'high-performance' });
    this.renderer.setSize(window.innerWidth, window.innerHeight);
    this.renderer.shadowMap.enabled = true; // Enable shadow mapping
    this.camera.position.z = 2;
    this.camera.position.x = 20;
    this.camera.rotation.y = 30;
    this.scene.position.z = -7;

    // Add ambient light
    const ambientLight = new THREE.AmbientLight(0xffffff, 2); // Soft white light
    this.scene.add(ambientLight);

    // Add directional light
    this.directionalLight  = new THREE.DirectionalLight(0xffffff, 4); // White light
    this.directionalLight.position.set(40, 15, 25);
    this.directionalLight.castShadow = true; // Enable shadows for the light
    this.directionalLight.shadow.mapSize.width = 2048; // Increase shadow map size
    this.directionalLight.shadow.mapSize.height = 2048; // Increase shadow map size
    this.directionalLight.shadow.radius = 1; // Soften shadow edges

    // Configure shadow camera
    this.directionalLight.shadow.camera.near = 0.5; // Near clipping plane
    this.directionalLight.shadow.camera.far = 500; // Far clipping plane
    this.directionalLight.shadow.camera.left = -50; // Left extent
    this.directionalLight.shadow.camera.right = 50; // Right extent
    this.directionalLight.shadow.camera.top = 50; // Top extent
    this.directionalLight.shadow.camera.bottom = -50; // Bottom extent
    this.directionalLight.shadow.bias = -0.0001; // Reduce shadow artifacts

    this.scene.add(this.directionalLight);

    // Add additional point lights
    const pointLight1 = new THREE.PointLight(0xffffff, 2);
    pointLight1.position.set(10, 10, 10);
    this.scene.add(pointLight1);

    const pointLight2 = new THREE.PointLight(0xffffff, 2);
    pointLight2.position.set(-10, 10, 10);
    this.scene.add(pointLight2);

    // Add a white floor
    const floorGeometry = new THREE.CircleGeometry(80, 32);
    const floorMaterial = new THREE.MeshStandardMaterial({ color: "white" });
    const floor = new THREE.Mesh(floorGeometry, floorMaterial);
    floor.rotation.x = -Math.PI / 2; // Rotate the floor to be horizontal
    floor.position.y = -9; // Position the floor below the model
    floor.receiveShadow = true; // Enable the floor to receive shadows
    this.scene.add(floor);
  }

  init(container: ElementRef): void {
    container.nativeElement.appendChild(this.renderer.domElement);
    this.controls = new OrbitControls(this.camera, this.renderer.domElement);
    this.controls.enableDamping = true; // Enable damping (inertia)
    this.controls.dampingFactor = 0.2; // Damping factor

    container.nativeElement.appendChild(this.renderer.domElement);
    const loader = new GLTFLoader();
    loader.load('./assets/tadiranLogo5.glb', (gltf) => {
      this.model = gltf.scene;
      //this.applyMaterial(this.model);

      const textureLoader = new THREE.TextureLoader();
      const texture = textureLoader.load('./assets/mv2.avif'); // Load your texture

      const material = new THREE.MeshStandardMaterial({
        //map: texture, // Apply the texture to the material
        color: "#2e8ae7", // Dark blue
        metalness: 0.4, // High metalness for a shiny look
        roughness: 0.2, // Low roughness for a glossy surface
        side: THREE.DoubleSide
      });

      const material2 = new THREE.MeshPhongMaterial({
        map: texture, // Apply the texture to the material
        //color: "#2e8ae7", // Dark blue
        reflectivity: 0.9,  // High metalness for a shiny look
        //roughness: 0.2, // Low roughness for a glossy surface
        side: THREE.DoubleSide
      })



      // Apply EdgeSplit Modifier
      //const modifier = new SimplifyModifier();
      const cutOffAngle = 0.5; // Adjust the cut-off angle as needed
      const tryKeepNormals = true;
      this.model.traverse((child) => {
        if ((child as THREE.Mesh).isMesh) {
          const mesh = child as THREE.Mesh;
          mesh.geometry = new EdgeSplitModifier().modify(mesh.geometry  as THREE.BufferGeometry, cutOffAngle, tryKeepNormals);
          mesh.geometry.computeVertexNormals(); // Recompute normals for smooth shading


          mesh.castShadow = true;
          mesh.receiveShadow = true;
          mesh.geometry.computeVertexNormals();
          mesh.material = material;
        }
        if ((child as THREE.Group).isGroup) {

        }
      });

      // Apply Smooth Shading
      /*this.model.traverse((child) => {
        if ((child as THREE.Mesh).isMesh) {
          const mesh = child as THREE.Mesh;
          mesh.geometry.computeVertexNormals();
        }
      });*/

      // Align the model's X-axis with the direction to the light
      this.lightDirection = new THREE.Vector3()
        .subVectors(this.directionalLight.position, this.model.position).normalize();
      const up = new THREE.Vector3(1, 0, 0);
      const quaternion = new THREE.Quaternion().setFromUnitVectors(up, this.lightDirection);
      this.model.setRotationFromQuaternion(quaternion);

      this.scene.add(this.model);

      // Create and add the arrow helpers for X, Y, Z axes
      //const length = 5; // Length of the arrows
      //this.arrowHelperX = new THREE.ArrowHelper(new THREE.Vector3(1, 0, 0), new THREE.Vector3(0, 0, 0), length, 0xff0000); // Red for X axis
      //this.arrowHelperY = new THREE.ArrowHelper(new THREE.Vector3(0, 1, 0), new THREE.Vector3(0, 0, 0), length, 0x00ff00); // Green for Y axis
      //this.arrowHelperZ = new THREE.ArrowHelper(new THREE.Vector3(0, 0, 1), new THREE.Vector3(0, 0, 0), length, 0x0000ff); // Blue for Z axis
      //this.scene.add(this.arrowHelperX);
      //this.scene.add(this.arrowHelperY);
      //this.scene.add(this.arrowHelperZ);

      // Load font and create text geometry
      //const fontLoader = new FontLoader();
      //fontLoader.load('https://threejs.org/examples/fonts/helvetiker_regular.typeface.json', (font) => {
      //  const textGeometry = new TextGeometry('Rotation: 0°', {
      //    font: font,
      //    size: 1,
      //    height: 0.1,
      //  });
      //  const textMaterial = new THREE.MeshBasicMaterial({ color: 0x000000 });
      //  this.textMesh = new THREE.Mesh(textGeometry, textMaterial);
      //  this.textMesh.position.set(0, 5, 0);
      //  this.scene.add(this.textMesh);
      //});

      this.animate();
    });
  }

  changeFloorTexture(textureUrl: string): void {
    const textureLoader = new THREE.TextureLoader();
    textureLoader.load(textureUrl, (texture) => {
      if (this.floor) {
        (this.floor.material as THREE.MeshStandardMaterial).map = texture;
        (this.floor.material as THREE.MeshStandardMaterial).needsUpdate = true;
      }
    });
  }
/*
  private applyMaterial(model: THREE.Group): void {
    const textureLoader = new THREE.TextureLoader();
    const texture = textureLoader.load('./assets/mv2.avif'); // Load your texture

    const material = new THREE.MeshStandardMaterial({
      map: texture, // Apply the texture to the material
      //color: "#2e8ae7", // Dark blue
      metalness: 0.4, // High metalness for a shiny look
      roughness: 0.2, // Low roughness for a glossy surface
      side: THREE.DoubleSide
    });

    model.traverse((child) => {
      if ((child as THREE.Mesh).isMesh) {
        (child as THREE.Mesh).material = material;
      }
    });
  }
*/
  private animate(): void {
    requestAnimationFrame(() => this.animate());
    if (this.model) {
      // Rotate the model around its tilted X-axis
      const tiltedXAxis = new THREE.Vector3(0, 1, 0).applyQuaternion(this.model.quaternion);
      this.model.rotateOnWorldAxis(tiltedXAxis, 0.001);

      // Update the arrow helpers' positions to match the model's position
      //this.arrowHelperX!.position.copy(this.model.position);
      //this.arrowHelperY!.position.copy(this.model.position);
      //this.arrowHelperZ!.position.copy(this.model.position);

      // Update the text with the current rotation angle
      //const rotationAngle = this.model.rotation.x * (180 / Math.PI); // Convert radians to degrees
      //if (this.textMesh) {
      //  (this.textMesh.geometry as TextGeometry).dispose();
      //  this.textMesh.geometry = new TextGeometry(`Rotation: ${rotationAngle.toFixed(2)}°`, {
      //    font: (this.textMesh.geometry as TextGeometry).parameters.options.font,
      //    size: 1,
      //    height: 0.1,
      //  });
      //}
    }
    if (this.controls) {
      this.controls.update(); // Update controls
    }
    this.renderer.render(this.scene, this.camera);
  }
}
