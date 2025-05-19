package tadiran.webnla.accdashboard.services;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.krysalis.jcharts.Chart;
import org.krysalis.jcharts.axisChart.AxisChart;
import org.krysalis.jcharts.axisChart.customRenderers.axisValue.renderers.ValueLabelPosition;
import org.krysalis.jcharts.axisChart.customRenderers.axisValue.renderers.ValueLabelRenderer;
import org.krysalis.jcharts.chartData.AxisChartDataSet;
import org.krysalis.jcharts.chartData.ChartDataException;
import org.krysalis.jcharts.chartData.DataSeries;
import org.krysalis.jcharts.chartData.PieChartDataSet;
import org.krysalis.jcharts.nonAxisChart.PieChart3D;
import org.krysalis.jcharts.properties.*;
import org.krysalis.jcharts.properties.util.ChartFont;
import org.krysalis.jcharts.properties.util.ChartStroke;
import org.krysalis.jcharts.types.ChartType;
import org.krysalis.jcharts.types.PieLabelType;
import org.springframework.stereotype.Service;
import tadiran.webnla.NLAContext;
import tadiran.webnla.bean.AbsDataClass;
import tadiran.webnla.bean.DataItem;
import tadiran.webnla.servlet.DrawChart;
import tadiran.webnla.tag.LocaleMessage;

import jakarta.servlet.http.HttpServletRequest;
import java.awt.*;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.Arrays;
import java.util.HashMap;

@Service
public class ChartService {

    private static final Logger logger = LogManager.getLogger(ChartService.class);

    private transient NLAContext ctx = NLAContext.getInstance();
    private DrawChart drawChart = new DrawChart();
    private static final Paint[] availableAltPaints = {new Color(200, 200, 200, 220),
            new Color(250, 250, 250, 220)};
    private static final Color[] availablePaints = {new Color(51, 153, 255, 100),  //Light Blue
            new Color(0, 255, 51, 100),    //Light Green
            new Color(255, 51, 51, 100),   //Light Red
            new Color(255, 204, 0, 100),   //Dark Yellow
            new Color(255, 0, 255, 100),   //Magenta
            new Color(153, 153, 153, 100), //Gray
            new Color(153, 102, 0, 100),   //Light Brown
            new Color(255, 153, 0, 100),   //Light Orange
            new Color(150, 150, 2),
            new Color(2, 0, 115),
            new Color(2, 150, 206),
            new Color(255, 130, 130),
            new Color(115, 2, 0),
            new Color(75, 2, 206),
            new Color(115, 2, 75)};

    private static PieChart3DProperties pieChart3DProperties = new PieChart3DProperties();
    private static LegendProperties legendProperties = new LegendProperties();
    private static ChartProperties chartProperties = new ChartProperties();
    private static AxisProperties axisProperties = new AxisProperties();

    static {
        pieChart3DProperties.setPieLabelType(PieLabelType.VALUE_LABELS);
        pieChart3DProperties.setZeroDegreeOffset(90);
        pieChart3DProperties.setValueLabelFont(ChartFont.DEFAULT_AXIS_VALUE);
        pieChart3DProperties.setTickLength(4f);
        pieChart3DProperties.setBorderChartStroke(new ChartStroke(new BasicStroke(1.2f), new Color(240, 240, 240)));

        legendProperties.setPlacement(LegendProperties.RIGHT);
        legendProperties.setChartPadding(25);
        legendProperties.setNumColumns(1);
        legendProperties.setBorderStroke(null);

        chartProperties.setEdgePadding(0);
        chartProperties.setTitlePadding(0);
        chartProperties.setValidate(false);

        AxisTypeProperties xAxisProps = axisProperties.getXAxisProperties();
        axisProperties.setBackgroundPaint(new Color(245, 245, 245, 200));
        ChartFont xScaleChartFont = new ChartFont(new Font(xAxisProps.getScaleChartFont().getFont().getFontName(), Font.PLAIN, 9), Color.black);
        xAxisProps.setScaleChartFont(xScaleChartFont);
        xAxisProps.setShowEndBorder(false);
        xAxisProps.setPaddingBetweenXAxisLabels(0);

        DataAxisProperties dataAxisProperties = (DataAxisProperties) axisProperties.getYAxisProperties();
        dataAxisProperties.setNumItems(10);
        dataAxisProperties.setShowGridLines(AxisTypeProperties.GRID_LINES_ONLY_WITH_LABELS);
        dataAxisProperties.setShowEndBorder(false);
    }

    public Chart getChart(HttpServletRequest request) throws ChartDataException, IOException, InterruptedException {
        //String type = request.getParameter("type");
        //String keys = request.getParameter("keys");
        //String size = request.getParameter("size");


        request.getSession(true);
        Chart chart;

        // prepare common chart params
        String title=request.getParameter("title");
        if ("".equals(title))
            title=null;
        request.setAttribute("title",title);

        String keys=request.getParameter("keys");
        if (keys==null)
            return null;
        request.setAttribute("fields",keys.split("[ ]*,[ ]*"));

        String size=request.getParameter("size");
        if ("".equals(size))
            size=null;

        String refObjectName = request.getParameter("refObjectName");
        // Note: refObjectName can be null as per controller's @RequestParam(required = false)
        // Subsequent methods like getChartDataSet and getPieChartDataSet (if used) should handle null if it's problematic for them.

        Dimension chartSize;
        chart=null;
        String type=request.getParameter("type");
        logger.debug("DrewChart - DoGet: Chart Type " + type + " <" + keys + ">, size " + size + ", refObjectName: " + refObjectName);

        if (type.equals("bar") && keys.indexOf("-") > 0) // Assuming keys is not null due to check above
            type = "bar-stack";
        chartSize=getSize(size,new Dimension(300,200));

        if (type.equals("3Dpie")) {
            // Assuming drawChart.getPieChartDataSet handles null refObjectName if it uses it.
            // If refObjectName is critical here, it should be checked before calling.
            // The ChartService's own getPieChartDataSet method has been updated to handle null refObjectName.
            PieChartDataSet pieChartDataSet = getPieChartDataSet(request);
            chart = new PieChart3D(pieChartDataSet, legendProperties, chartProperties, chartSize.width, chartSize.height);
        }
        else if (type.equals("area")) {
            DataSeries dataSeries = getChartDataSet(request, ChartType.AREA); // getChartDataSet will now throw ChartDataException if refObjectName is null
            LegendProperties legendProp = null;
            if (refObjectName != null && refObjectName.indexOf(",") > 0) {
                legendProp = legendProperties;
            }
            chart = new AxisChart(dataSeries, chartProperties, axisProperties, legendProp, chartSize.width, chartSize.height);
        }
        else if (type.equals("line")) {
            DataSeries dataSeries = getChartDataSet(request, ChartType.LINE); // getChartDataSet will now throw ChartDataException if refObjectName is null
            LegendProperties legendProp = null;
            if (refObjectName != null && refObjectName.indexOf(",") > 0) {
                legendProp = legendProperties;
            }
            chart = new AxisChart(dataSeries, chartProperties, axisProperties, legendProp, chartSize.width, chartSize.height);
        }
        else if (type.equals("bar")) {
            LegendProperties legendProp = null;
            DataSeries dataSeries;
            // getChartDataSet will now throw ChartDataException if refObjectName is null
            if (refObjectName != null && refObjectName.indexOf(",") > 0) {
                legendProp = legendProperties;
                dataSeries = getChartDataSet(request, ChartType.BAR_CLUSTERED);
            } else {
                dataSeries = getChartDataSet(request, ChartType.BAR);
            }
            chart = new AxisChart(dataSeries, chartProperties, axisProperties, legendProp, chartSize.width, chartSize.height);
        }
        else if (type.equals("bar-stack")) {
            DataSeries dataSeries = getChartDataSet(request, ChartType.BAR_STACKED);
            chart = new AxisChart(dataSeries, chartProperties, axisProperties, null, chartSize.width, chartSize.height);
        }

        return chart;
    }

    private DataSeries getChartDataSet(HttpServletRequest request, ChartType type) throws ChartDataException, UnsupportedEncodingException, InterruptedException {
        String langName = (String) request.getSession().getAttribute("Accept-Language");
        String title;

        String refObjectNameParam = request.getParameter("refObjectName");
        if (refObjectNameParam == null) {
            logger.error("refObjectName parameter is null. This parameter is required for chart dataset generation.");
            throw new ChartDataException("refObjectName parameter is missing for chart dataset generation.");
        }

        if (type == ChartType.BAR || type == ChartType.BAR_CLUSTERED || type == ChartType.BAR_STACKED)
            title = LocaleMessage.getMessage((String) request.getAttribute("title"), langName);
        else
            title = (String) request.getAttribute("title");

        String[] fields = (String[]) request.getAttribute("fields");
        String[] legendLabels = refObjectNameParam.split("[ ]*,[ ]*");

        String[] fields0 = fields[0].split("-");
        if (fields0.length > legendLabels.length) {
            String prevLegendLabel = legendLabels[0];
            legendLabels = new String[fields0.length];
            legendLabels[0] = prevLegendLabel;
            for (int i = 1; i < fields0.length; i++)
                legendLabels[i] = legendLabels[0];
        }

        String[] labels = new String[fields.length];
        double[][] data = new double[legendLabels.length][fields.length];

        AbsDataClass obj;
        HashMap<String, DataItem> objData;
        String objName;
        String[] field;
        for (int r = 0; r < legendLabels.length; r++) {
            obj = ctx.getObject(legendLabels[r]);
            objData = obj.getData();
            objName = obj.getName();
            if (objName != null && !objName.equals(legendLabels[r]))
                legendLabels[r] = objName;

            for (int i = 0; i < fields.length; i++) {
                DataItem item;
                field = fields[i].split("-");
                String ersKey = "null";
                if (field.length == 1) {
                    ersKey = fields[i];
                } else {
                    if (r < field.length)
                        ersKey = field[r];
                }

                item = objData.get(ersKey);
                String itemName = "null";
                if (item == null) {
                    item = new DataItem(ersKey, (ersKey.length() > 0) ? ersKey : itemName, 0);
                } else {
                    if (!(field.length == 1 && r > 0)) {
                        itemName = item.getName(langName);
                    }
                }

                data[r][i] = Double.valueOf(item.getValue().toString());

                if (r == 0) {
                    labels[i] = itemName;
                }
            }
        }

        Paint[] colors = getColors(legendLabels.length);
        ChartTypeProperties chartTypeProperties = null;
        if (type == ChartType.AREA) {
            chartTypeProperties = new AreaChartProperties();

            ValueLabelRenderer valueLabelRenderer = new ValueLabelRenderer(false, false, false, 0);
            valueLabelRenderer.setValueLabelPosition(ValueLabelPosition.AXIS_TOP);
            valueLabelRenderer.useVerticalLabels(false);
            ((AreaChartProperties) chartTypeProperties).addPostRenderEventListener(valueLabelRenderer);
        } else if (type == ChartType.LINE) {
            Stroke[] strokes = {LineChartProperties.DEFAULT_LINE_STROKE, LineChartProperties.DEFAULT_LINE_STROKE};
            Shape[] shapes = {PointChartProperties.SHAPE_CIRCLE, PointChartProperties.SHAPE_CIRCLE};
            chartTypeProperties = new LineChartProperties(strokes, shapes);
        } else if (type == ChartType.BAR) {
            ValueLabelRenderer valueLabelRenderer = new ValueLabelRenderer(false, false, false, 0);
            valueLabelRenderer.setValueLabelPosition(ValueLabelPosition.ON_TOP);
            valueLabelRenderer.useVerticalLabels(false);

            if (legendLabels.length == 1 && fields.length > 1) {
                double[][] dataOld = data.clone();
                data = new double[fields.length][fields.length];
                for (int i = 0; i < labels.length; i++)
                    if (labels[i].length() > 9)
                        labels[i] = labels[i].substring(0, 8) + "..";

                for (int i = 0; i < fields.length; i++)
                    data[i][i] = dataOld[0][i];

                colors = getColors(fields.length);
                type = ChartType.BAR_STACKED;
                chartTypeProperties = new StackedBarChartProperties();
            } else {
                chartTypeProperties = new BarChartProperties();
                ((BarChartProperties) chartTypeProperties).addPostRenderEventListener(valueLabelRenderer);
            }
        } else if (type == ChartType.BAR_STACKED) {
            chartTypeProperties = new StackedBarChartProperties();
            if (legendLabels.length > 1)
                colors = getAltColors(legendLabels.length);
        } else if (type == ChartType.BAR_CLUSTERED) {
            chartTypeProperties = new ClusteredBarChartProperties();

            ValueLabelRenderer valueLabelRenderer = new ValueLabelRenderer(false, false, false, 0);
            valueLabelRenderer.setValueLabelPosition(ValueLabelPosition.ON_TOP);
            valueLabelRenderer.useVerticalLabels(false);
            ((ClusteredBarChartProperties) chartTypeProperties).addPostRenderEventListener(valueLabelRenderer);
        }

        AxisChartDataSet axisChartDataSet = new AxisChartDataSet(data, legendLabels, colors, type, chartTypeProperties);

        String decode_title;
        if (title != null)
            decode_title = URLDecoder.decode(title, "UTF-8");
        else
            decode_title = title;

        DataSeries dataSeries = new DataSeries(labels, null, null, title);
        dataSeries.addIAxisPlotDataSet(axisChartDataSet);

        return dataSeries;
    }


    private PieChartDataSet getPieChartDataSet(HttpServletRequest request) throws ChartDataException, UnsupportedEncodingException, InterruptedException {
        //String langName = (String) request.getSession().getAttribute("Accept-Language");
        //String title = LocaleMessage.getMessage((String) request.getAttribute("title"), langName);
        //String[] fields = (String[]) request.getAttribute("fields");

        // prepare common chart params
        String title=request.getParameter("title");
        if ("".equals(title))
            title=null;
        //request.setAttribute("title",title);

        String keys=request.getParameter("keys");
        if (keys==null)
            return null;
        String[] fields = keys.split("[ ]*,[ ]*");
        //request.setAttribute("fields",keys.split("[ ]*,[ ]*"));

        String size=request.getParameter("size");
        if ("".equals(size))
            size=null;

        String langName=(String)request.getSession().getAttribute("Accept-Language");
        //String title=LocaleMessage.getMessage((String)request.getAttribute("title"),langName);

        //String title=(String)request.getAttribute("title");
        //String[] fields=(String[])request.getAttribute("fields");

        double[] data=new double[fields.length];
        String[] labels=new String[fields.length];
        int[] map=new int[fields.length];

        String refObjectNameParam = request.getParameter("refObjectName");
        if (refObjectNameParam == null) {
            logger.error("refObjectName parameter is null. This parameter is required for pie chart dataset generation.");
            throw new ChartDataException("refObjectName parameter is missing for pie chart dataset generation.");
        }
        AbsDataClass obj = ctx.getObject(refObjectNameParam);
        if (obj == null) {
            logger.error("Could not find object for refObjectName: " + refObjectNameParam);
            throw new ChartDataException("Data object not found for refObjectName: " + refObjectNameParam);
        }
        HashMap<String, DataItem> objData = obj.getData();
        if (objData == null) {
             logger.error("Data map is null for refObjectName: " + refObjectNameParam);
             throw new ChartDataException("Data map is null for refObjectName: " + refObjectNameParam);
        }
        DataItem item;
        double sum = 0;
        int a = 0;
        int b = fields.length - 1;
        for (int i = 0; i < fields.length; i++) {
            item = objData.get(fields[i]);
            if (item == null) {
                item = new DataItem(fields[i], "...", 0);
            }
            double val = Double.valueOf(item.getValue().toString());
            sum += val;
            if (val != 0) {
                data[a] = val;
                labels[a] = item.getName(langName);
                map[i] = a;
                a++;
            } else {
                labels[b] = item.getName(langName);
                map[i] = b;
                b--;
            }
        }

        Paint[] colors = getColors(map);
        if (sum == 0) {
            if (fields.length < 6) {
                data = new double[data.length + 1];
                data[data.length - 1] = 0.1;
                colors = Arrays.copyOf(colors, colors.length + 1);
                colors[colors.length - 1] = availablePaints[5];
                labels = Arrays.copyOf(labels, labels.length + 1);
                labels[labels.length - 1] = "no data";
            } else {
                data[map[5]] = 0.1;
            }
            pieChart3DProperties.setPieLabelType(PieLabelType.NO_LABELS);
        } else {
            pieChart3DProperties.setPieLabelType(PieLabelType.VALUE_LABELS);
        }

        String decode_title;
        String encode_title;
        if (title != null) {
            decode_title = URLDecoder.decode(title, "UTF-8");
            encode_title = URLEncoder.encode(title, "UTF-8");
        } else {
            decode_title = title;
            encode_title = title;
        }

        return new PieChartDataSet(decode_title, data, labels, colors, pieChart3DProperties);
    }


    private Paint[] getColors(int[] map) {
        Paint[] colors;
        int size = map.length;
        if (size > 0) {
            colors = new Paint[size];
            for (int i = 0; i < size; i++) {
                colors[map[i]] = availablePaints[i];
            }
        } else {
            int ind = -size - 1;
            colors = new Paint[-size];
            for (int i = 0; i < -size; i++) {
                colors[i] = availablePaints[ind - i];
            }
        }

        return colors;
    }


    private Paint[] getColors(int size) {
        Paint[] colors;
        if (size > 0) {
            colors = new Paint[size];
            for (int i = 0; i < size; i++) {
                colors[i] = availablePaints[i];
            }
        } else {
            int ind = -size - 1;
            colors = new Paint[-size];
            for (int i = 0; i < -size; i++) {
                colors[i] = availablePaints[ind - i];
            }
        }

        return colors;
    }


    private Paint[] getAltColors(int num) {
        Paint[] colors = new Paint[num];
        for (int i = 0; i < num; i++)
            colors[i] = availableAltPaints[i];

        return colors;
    }


    private Dimension getSize(String size, Dimension defaultSize) {
        if (size == null)
            return defaultSize;
        else {
            String[] dim = size.split("x");
            return new Dimension(Integer.parseInt(dim[0]), Integer.parseInt(dim[1]));
        }
    }
}
