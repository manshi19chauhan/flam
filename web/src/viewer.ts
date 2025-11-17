// web/src/viewer.ts
interface FrameData {
    imageData: string;
    width: number;
    height: number;
    fps: number;
    processingTime: number;
    mode: string;
    timestamp: number;
}

class EdgeDetectionViewer {
    private canvas: HTMLCanvasElement;
    private ctx: CanvasRenderingContext2D;
    private statsElement: HTMLElement;
    private modeElement: HTMLElement;
    private currentMode: string = 'canny';
    
    constructor() {
        this.canvas = document.getElementById('preview') as HTMLCanvasElement;
        this.ctx = this.canvas.getContext('2d')!;
        this.statsElement = document.getElementById('stats')!;
        this.modeElement = document.getElementById('mode')!;
        
        this.initializeViewer();
    }
    
    private initializeViewer(): void {
        this.setupEventListeners();
        this.loadSampleFrame();
        this.setupWebSocketMock();
    }
    
    private setupEventListeners(): void {
        const modeSelect = document.getElementById('mode-select') as HTMLSelectElement;
        const captureBtn = document.getElementById('capture-btn') as HTMLButtonElement;
        
        modeSelect.addEventListener('change', (e) => {
            this.currentMode = (e.target as HTMLSelectElement).value;
            this.updateMode(this.currentMode);
        });
        
        captureBtn.addEventListener('click', () => {
            this.captureFrame();
        });
    }
    
    private loadSampleFrame(): void {
        // Create a sample edge detection result
        this.generateSampleFrame();
    }
    
    private generateSampleFrame(): void {
        const width = 640;
        const height = 480;
        
        this.canvas.width = width;
        this.canvas.height = height;
        
        // Create a gradient background
        const gradient = this.ctx.createLinearGradient(0, 0, width, height);
        gradient.addColorStop(0, '#1a1a1a');
        gradient.addColorStop(1, '#2d2d2d');
        
        this.ctx.fillStyle = gradient;
        this.ctx.fillRect(0, 0, width, height);
        
        // Draw sample edges
        this.drawSampleEdges();
        
        // Update stats with sample data
        this.updateStats({
            imageData: '',
            width: width,
            height: height,
            fps: 15.5,
            processingTime: 45.2,
            mode: this.currentMode,
            timestamp: Date.now()
        });
    }
    
    private drawSampleEdges(): void {
        this.ctx.strokeStyle = '#00ff00';
        this.ctx.lineWidth = 2;
        
        // Draw some sample edge patterns
        for (let i = 0; i < 20; i++) {
            const x1 = Math.random() * this.canvas.width;
            const y1 = Math.random() * this.canvas.height;
            const x2 = x1 + Math.random() * 100 - 50;
            const y2 = y1 + Math.random() * 100 - 50;
            
            this.ctx.beginPath();
            this.ctx.moveTo(x1, y1);
            this.ctx.lineTo(x2, y2);
            this.ctx.stroke();
        }
        
        // Draw a sample shape
        this.ctx.strokeStyle = '#ff0000';
        this.ctx.beginPath();
        this.ctx.arc(this.canvas.width / 2, this.canvas.height / 2, 80, 0, 2 * Math.PI);
        this.ctx.stroke();
    }
    
    private setupWebSocketMock(): void {
        // Simulate real-time updates
        setInterval(() => {
            this.simulateFrameUpdate();
        }, 2000);
    }
    
    private simulateFrameUpdate(): void {
        this.drawSampleEdges();
        
        this.updateStats({
            imageData: '',
            width: this.canvas.width,
            height: this.canvas.height,
            fps: 14.5 + Math.random() * 2,
            processingTime: 40 + Math.random() * 15,
            mode: this.currentMode,
            timestamp: Date.now()
        });
    }
    
    public updateFrame(frameData: FrameData): void {
        if (frameData.imageData) {
            const img = new Image();
            img.onload = () => {
                this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
                this.ctx.drawImage(img, 0, 0, this.canvas.width, this.canvas.height);
                this.updateStats(frameData);
            };
            img.src = `data:image/png;base64,${frameData.imageData}`;
        }
    }
    
    private updateStats(frameData: FrameData): void {
        this.statsElement.innerHTML = `
            <div><strong>Resolution:</strong> ${frameData.width}x${frameData.height}</div>
            <div><strong>FPS:</strong> ${frameData.fps.toFixed(1)}</div>
            <div><strong>Processing Time:</strong> ${frameData.processingTime.toFixed(1)}ms</div>
            <div><strong>Timestamp:</strong> ${new Date(frameData.timestamp).toLocaleTimeString()}</div>
        `;
        this.modeElement.textContent = `Current Mode: ${frameData.mode.toUpperCase()}`;
    }
    
    private updateMode(mode: string): void {
        console.log(`Mode changed to: ${mode}`);
        this.currentMode = mode;
        this.generateSampleFrame();
        
        // In real implementation, send mode change to Android app
        this.simulateModeChange(mode);
    }
    
    private simulateModeChange(mode: string): void {
        // Simulate network request to change processing mode
        fetch(`/api/mode?mode=${mode}`, { method: 'POST' })
            .then(response => console.log(`Mode ${mode} set successfully`))
            .catch(error => console.error('Failed to set mode:', error));
    }
    
    public captureFrame(): string {
        const dataUrl = this.canvas.toDataURL('image/png');
        const link = document.createElement('a');
        link.download = `edge-frame-${new Date().getTime()}.png`;
        link.href = dataUrl;
        link.click();
        return dataUrl;
    }
}

// Initialize when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    const viewer = new EdgeDetectionViewer();
    // Make it globally available for capture button
    (window as any).viewer = viewer;
});