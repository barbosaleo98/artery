import artery.application.ItsG5Service;

simple AlertReceiverService like ItsG5Service
{
    parameters:
        double safeDist @unit(m) = default(200.0 m);
}