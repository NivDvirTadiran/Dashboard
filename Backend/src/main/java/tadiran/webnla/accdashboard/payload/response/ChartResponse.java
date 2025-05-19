package tadiran.webnla.accdashboard.payload.response;

import java.util.Arrays;

public class ChartResponse {

    private byte[] chartData;

    public ChartResponse(byte[] chartData) {
        this.chartData = chartData;
    }

    public byte[] getChartData() {
        return chartData;
    }

    public void setChartData(byte[] chartData) {
        this.chartData = chartData;
    }

    @Override
    public String toString() {
        return "ChartResponse{" +
                "chartData=" + Arrays.toString(chartData) +
                '}';
    }
}
