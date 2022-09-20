import artery.inet.World;
import inet.visualizer.contract.IPhysicalEnvironmentVisualizer;

network CanvasWorld extends World
{
    @display("bgb=1700,1600");
    submodules:
    canvasVisualizer: <default("PhysicalEnvironmentCanvasVisualizer")> like IPhysicalEnvironmentVisualizer if withPhysicalEnvironment {
        parameters:
            @display("p=100,70");       
    }
}