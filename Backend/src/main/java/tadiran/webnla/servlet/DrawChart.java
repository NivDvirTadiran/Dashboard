package tadiran.webnla.servlet;

import org.krysalis.jcharts.encoders.PNGEncoder;
import org.krysalis.jcharts.encoders.ServletEncoderHelper;
import tadiran.webnla.NLAContext;
import tadiran.webnla.bean.AbsDataClass;
import tadiran.webnla.bean.DataItem;
import java.awt.*;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;
import java.util.HashMap;
import java.net.URLEncoder;
import java.net.URLDecoder;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;
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
import tadiran.webnla.tag.LocaleMessage;

import static org.krysalis.jcharts.encoders.ServletEncoderHelper.encodePNG;


/**
 *
 * @author evgeniv
 */
public class DrawChart extends HttpServlet {
    
    //private static final Logger log = LogManager.getLogger("NLAData");
    private static final Logger log = LogManager.getLogger("DrawChart");
    private transient NLAContext ctx=NLAContext.getInstance();
    private static final Paint[] availableAltPaints = { new Color(200,200,200,220),
                                                        new Color(250,250,250,220) };
    private static final Color[] availablePaints = {new Color(51,153,255,100),  //Light Blue
                                                    new Color(0,255,51,100),    //Light Green
                                                    new Color(255,51,51,100),   //Light Red
                                                    new Color(255,204,0,100),   //Dark Yellow
                                                    new Color(255,0,255,100),   //Magenta
                                                    new Color(153,153,153,100), //Gray
                                                    new Color(153,102,0,100),   //Light Brown
                                                    new Color(255,153,0,100),   //Light Orange
                                                    new Color(150,150,2),
                                                    new Color(2,0,115),
                                                    new Color(2,150,206),
                                                    new Color(255,130,130),
                                                    new Color(115,2,0),
                                                    new Color(75,2,206),
                                                    new Color(115,2,75)};
//  Original Colors
//    private static final Color[] availablePaints = {new Color(0,0,255,100),     //Blue
//                                                    new Color(0,200,0,100),     //Green
//                                                    new Color(255,0,0,100),     //Red
//                                                    new Color(255,255,0,100),   //Yellow
//                                                    new Color(255,0,255,100),   //Magenta
//                                                    new Color(250,250,250,100),
//                                                    new Color(0,115,2),
//                                                    new Color(206,75,2),
//                                                    new Color(150,150,2),
//                                                    new Color(2,0,115),
//                                                    new Color(2,150,206),
//                                                    new Color(255,130,130),
//                                                    new Color(115,2,0),
//                                                    new Color(75,2,206),
//                                                    new Color(115,2,75)};

//    private static final Color[] availablePaints = {new Color(65,105,225,100),  //royal blue        #4169E1 (65,105,225)    old - new Color(0,0,255,200) - Blue
//                                                    new Color(152,251,152,100), //pale green        #98FB98 (152,251,152)   old - new Color(0,200,0,200) - Green
//                                                    new Color(199,21,133,100),  //medium violet red #C71585 (199,21,133)    old - new Color(255,0,0,200) - RED
//                                                    new Color(218,165,32,100),  //golden rod        #DAA520 (218,165,32)    old - new Color(255,255,0)   - Yellow
//                                                    new Color(255,182,193,100), //light pink        #FFB6C1 (255,182,193)   old - new Color(255,0,255)   - Magenta
//                                                    new Color(250,250,250,100), //White
//                                                    new Color(60,179,113,100),  //medium sea green  #3CB371 (60,179,113)    old - new Color(0,115,2)     - Dark Green
//                                                    new Color(206,75,2),
//                                                    new Color(150,150,2),
//                                                    new Color(2,0,115),
//                                                    new Color(2,150,206),
//                                                    new Color(255,130,130),
//                                                    new Color(115,2,0),
//                                                    new Color(75,2,206),
//                                                    new Color(115,2,75)};

    private static PieChart3DProperties pieChart3DProperties= new PieChart3DProperties();
    private static LegendProperties legendProperties=new LegendProperties();
    private static ChartProperties chartProperties=new ChartProperties();
    private static AxisProperties axisProperties= new AxisProperties();


    @Override
    public void init () throws ServletException {
        super.init();
        pieChart3DProperties.setPieLabelType(PieLabelType.VALUE_LABELS);
        pieChart3DProperties.setZeroDegreeOffset(90);
        pieChart3DProperties.setValueLabelFont(ChartFont.DEFAULT_AXIS_VALUE);
        pieChart3DProperties.setTickLength(4f);
        pieChart3DProperties.setBorderChartStroke(new ChartStroke(new BasicStroke(1.2f), new Color(240,240,240)));
                //new BasicStroke(0.5f), Color.GRAY));

        legendProperties.setPlacement(LegendProperties.RIGHT);
        legendProperties.setChartPadding(25);
        legendProperties.setNumColumns(1);
	//legendProperties.setChartFont(new ChartFont(new Font(legendProperties.getChartFont().getFont().getFontName(),Font.PLAIN, 11), Color.black));
        legendProperties.setBorderStroke(null);

        chartProperties.setEdgePadding(0);
        chartProperties.setTitlePadding(0);
        chartProperties.setValidate(false);

        //axisProperties.setXAxisLabelsAreVertical(true);
        AxisTypeProperties xAxisProps=axisProperties.getXAxisProperties();
        axisProperties.setBackgroundPaint(new Color(245,245,245,200));
        ChartFont xScaleChartFont= new ChartFont( new Font(xAxisProps.getScaleChartFont().getFont().getFontName(), Font.PLAIN, 9 ), Color.black );
        xAxisProps.setScaleChartFont( xScaleChartFont );
        //xAxisProps.setShowGridLines(AxisTypeProperties.GRID_LINES_ALL);
        xAxisProps.setShowEndBorder(false);
        xAxisProps.setPaddingBetweenXAxisLabels(0);

        DataAxisProperties dataAxisProperties= (DataAxisProperties) axisProperties.getYAxisProperties();
        dataAxisProperties.setNumItems(10);
        dataAxisProperties.setShowGridLines(AxisTypeProperties.GRID_LINES_ONLY_WITH_LABELS);
        dataAxisProperties.setShowEndBorder(false);
    }

    private DataSeries getChartDataSet(HttpServletRequest request, ChartType type) throws ChartDataException, UnsupportedEncodingException, InterruptedException {
        String langName=(String)request.getSession().getAttribute("Accept-Language");
        String title;
        
        if(type==ChartType.BAR || type==ChartType.BAR_CLUSTERED || type==ChartType.BAR_STACKED)
            title=LocaleMessage.getMessage((String)request.getAttribute("title"),langName);
        else
            title=(String)request.getAttribute("title");
        
        //String title=(String)request.getAttribute("title");
        String[] fields=(String[])request.getAttribute("fields");
        String[] legendLabels= request.getParameter("refObjectName").split("[ ]*,[ ]*");

        String[] fields0=fields[0].split("-");
        if (fields0.length>legendLabels.length) 
        {
            String prevLegendLabel=legendLabels[0];
            legendLabels=new String[fields0.length];
            legendLabels[0]=prevLegendLabel;
            for (int i=1; i<fields0.length; i++)
                legendLabels[i]=legendLabels[0];
        }

        String[] labels=new String[fields.length];
        double[][] data= new double[legendLabels.length][fields.length];

        AbsDataClass obj;
        HashMap<String,DataItem> objData;
        String objName;
        String[] field;
        for(int r=0; r<legendLabels.length; r++)
        {
            obj=ctx.getObject(legendLabels[r]);
            objData=obj.getData();
            objName=obj.getName();
            if (objName!=null && !objName.equals(legendLabels[r]))
                legendLabels[r]=objName;
			
            for (int i=0; i<fields.length; i++)
            {
                DataItem item;
                field=fields[i].split("-");
                String ersKey="null";
                if (field.length==1){
                    ersKey=fields[i];
                }
                else {
                    if (r<field.length)
                        ersKey=field[r];
                }

                item=objData.get(ersKey);
                String itemName="null";
                if (item==null) {
                    item=new DataItem(ersKey,(ersKey.length()>0)?ersKey:itemName,0);
                }
                else {
                    if (!(field.length==1 && r>0))
                    {
                        //itemName=item.getName();
                        itemName=item.getName(langName);
                        log.debug("getChartDataSet - itemName - " + itemName + ", encoded - " + URLEncoder.encode(itemName,"UTF-8") + ", decoded - " + URLDecoder.decode(itemName,"UTF-8") + ")");
                    }
                }

                data[r][i]=Double.valueOf(item.getValue().toString());

                if (r==0) {
                    labels[i]=itemName;
                }
//                else 
//                {
//                    if (!itemName.equals("null"))
//                    { //is second+ field defined
//                        labels[i]+="+"+itemName;
//
//                        if (r==legendLabels.length-1 && type==ChartType.BAR_STACKED)
//                        {
//                            double sum=0;
//                            for (int j=0; j<legendLabels.length-1; j++)
//                                sum+=data[j][i];
//                            data[r][i]-=sum;
//                        }
//                    }
//                }
            }
        }

        Paint [] colors=getColors(legendLabels.length);
        ChartTypeProperties chartTypeProperties=null;
        if (type==ChartType.AREA) 
        {
            chartTypeProperties= new AreaChartProperties();

            ValueLabelRenderer valueLabelRenderer = new ValueLabelRenderer( false, false, false,0 );
            valueLabelRenderer.setValueLabelPosition( ValueLabelPosition.AXIS_TOP);
            valueLabelRenderer.useVerticalLabels( false );
            ((AreaChartProperties)chartTypeProperties).addPostRenderEventListener( valueLabelRenderer );
        }
        else if (type==ChartType.LINE) 
        {
            Stroke[] strokes= { LineChartProperties.DEFAULT_LINE_STROKE, LineChartProperties.DEFAULT_LINE_STROKE };
            Shape[] shapes= { PointChartProperties.SHAPE_CIRCLE, PointChartProperties.SHAPE_CIRCLE };
            chartTypeProperties= new LineChartProperties( strokes, shapes );
        }
        else if (type==ChartType.BAR) 
        {
            ValueLabelRenderer valueLabelRenderer = new ValueLabelRenderer( false, false, false,0 );
            valueLabelRenderer.setValueLabelPosition( ValueLabelPosition.ON_TOP);
            valueLabelRenderer.useVerticalLabels( false );

            if (legendLabels.length==1 && fields.length>1) 
            {
                double[][] dataOld=data.clone();
                data=new double[fields.length][fields.length];
                for (int i=0; i<labels.length; i++)
                    if (labels[i].length()>9)
                        labels[i]=labels[i].substring(0,8)+"..";

                for (int i=0; i<fields.length; i++)
                    data[i][i]=dataOld[0][i];

                colors=getColors(fields.length);
                type=ChartType.BAR_STACKED;
                chartTypeProperties=new StackedBarChartProperties();
            }
            else
            {
                chartTypeProperties=new BarChartProperties();
                ((BarChartProperties)chartTypeProperties).addPostRenderEventListener( valueLabelRenderer );
            }
        }
        else if (type==ChartType.BAR_STACKED)
        {
            chartTypeProperties=new StackedBarChartProperties();
            if (legendLabels.length>1)
                colors=getAltColors(legendLabels.length);
        }
        else if (type==ChartType.BAR_CLUSTERED) 
        {
            chartTypeProperties=new ClusteredBarChartProperties();

            ValueLabelRenderer valueLabelRenderer = new ValueLabelRenderer( false, false, false,0 );
            valueLabelRenderer.setValueLabelPosition( ValueLabelPosition.ON_TOP);
            valueLabelRenderer.useVerticalLabels( false );
            ((ClusteredBarChartProperties)chartTypeProperties).addPostRenderEventListener( valueLabelRenderer );
        }

        AxisChartDataSet axisChartDataSet= new AxisChartDataSet(data, legendLabels, colors, type, chartTypeProperties);

        //7-Apr-2020 YR BZ#51038
        String decode_title;
        if(title != null)
            decode_title = URLDecoder.decode(title,"UTF-8");
        else
            decode_title = title;
        
        //DataSeries dataSeries = new DataSeries(labels, null, null, URLDecoder.decode(title,"UTF-8") );
        //DataSeries dataSeries = new DataSeries(labels, null, null, URLEncoder.encode(title,"UTF-8") );
        //DataSeries dataSeries = new DataSeries(labels, null, null, decode_title );
        DataSeries dataSeries = new DataSeries(labels, null, null, title );
        dataSeries.addIAxisPlotDataSet(axisChartDataSet);

        return dataSeries;
    }

    
    public PieChartDataSet getPieChartDataSet(HttpServletRequest request) throws ChartDataException, UnsupportedEncodingException, InterruptedException {
        String langName=(String)request.getSession().getAttribute("Accept-Language");
        String title=LocaleMessage.getMessage((String)request.getAttribute("title"),langName);
        
        //String title=(String)request.getAttribute("title");
        String[] fields=(String[])request.getAttribute("fields");

        double[] data=new double[fields.length];
        String[] labels=new String[fields.length];
        int[] map=new int[fields.length];

        AbsDataClass obj=ctx.getObject(request.getParameter("refObjectName"));
        HashMap<String,DataItem> objData=obj.getData();
        DataItem item;
        double sum=0;
        int a=0;
        int b=fields.length-1;
        for(int i=0; i<fields.length; i++) 
        {
            item=objData.get(fields[i]);
            if (item==null)
            {
                item=new DataItem(fields[i],"...",0);
            }
            double val=Double.valueOf(item.getValue().toString());
            sum+=val;
            if (val!=0)
            {
                data[a]=val;
                labels[a]=item.getName(langName);
                map[i]=a;
                a++;
            }
            else 
            {
                labels[b]=item.getName(langName);
                map[i]=b;
                b--;
            }
        }

        Paint[] colors=getColors(map);
        if (sum==0) 
        {
            if (fields.length<6)
            {
                data=new double[data.length+1]; data[data.length-1]=0.1;
                colors=Arrays.copyOf(colors, colors.length+1); colors[colors.length-1]=availablePaints[5];
                labels=Arrays.copyOf(labels,labels.length+1); labels[labels.length-1]="no data";
            }
            else {
                data[map[5]]=0.1;
            }
            pieChart3DProperties.setPieLabelType(PieLabelType.NO_LABELS);
        }
        else
        {
            pieChart3DProperties.setPieLabelType(PieLabelType.VALUE_LABELS);
        }

        //7-Apr-2020 YR BZ#51038
        String decode_title;
        String encode_title;
        if(title != null)
        {
            decode_title = URLDecoder.decode(title,"UTF-8");
            encode_title = URLEncoder.encode(title,"UTF-8");
        }
        else
        {
            decode_title = title;
            encode_title = title;
        }
        
        return new PieChartDataSet(decode_title, data, labels, colors, pieChart3DProperties);
    }

    
    private Paint [] getColors(int[] map)
    {
        Paint[] colors;
        int size=map.length;
        if (size>0) 
        {
            colors=new Paint[size];
            for (int i=0; i<size; i++) 
            {
                colors[map[i]]=availablePaints[i];
            }
        }
        else 
        {
            int ind=-size-1;
            colors=new Paint[-size];
            for (int i=0; i<-size; i++) {
                colors[i]=availablePaints[ind-i];
            }
        }

        return colors;
    }

    
    private Paint [] getColors(int size)
    {
        Paint[] colors;
        if (size>0)
        {
            colors=new Paint[size];
            for (int i=0; i<size; i++) {
                colors[i]=availablePaints[i];
            }
        }
        else 
        {
            int ind=-size-1;
            colors=new Paint[-size];
            for (int i=0; i<-size; i++) {
                colors[i]=availablePaints[ind-i];
            }
        }

        return colors;
    }

    
    private Paint[] getAltColors(int num) 
    {
        Paint[] colors=new Paint[num];
        for (int i=0; i<num; i++)
            colors[i]=availableAltPaints[i];

        return colors;
    }

    
    private Dimension getSize(String size, Dimension defaultSize) 
    {
        if (size==null)
            return defaultSize;
        else 
        {
            String[] dim=size.split("x");
            return new Dimension(Integer.parseInt(dim[0]),Integer.parseInt(dim[1]));
        }
    }

    /**
     * Handles the HTTP <code>GET</code> method.
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet (HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException 
    {
        request.getSession(true);
        Chart chart;

            
        // prepare common chart params
        String title=request.getParameter("title");
        if ("".equals(title))
            title=null;
        request.setAttribute("title",title);

        String keys=request.getParameter("keys");
        if (keys==null)
            return;
        request.setAttribute("fields",keys.split("[ ]*,[ ]*"));

        String size=request.getParameter("size");
        if ("".equals(size))
            size=null;

        // create chart by request
        try {
            Dimension chartSize;
            chart=null;
            String type=request.getParameter("type");
            log.debug("DrewChart - DoGet: Chart Type " + type + " <" + keys + ">, size " + size);

            if (type.equals("bar") && keys.indexOf("-")>0)
                type="bar-stack";

            if (type.equals("3Dpie"))
            {
                chartSize=getSize(size,new Dimension(300,200));
                PieChartDataSet pieChartDataSet = getPieChartDataSet(request);
                log.debug("Create 3Dpie Chart (title - " + pieChartDataSet.getChartTitle() + ")");
                chart = new PieChart3D( pieChartDataSet, legendProperties, chartProperties, chartSize.width, chartSize.height);
            }
            else if (type.equals("area"))
            {
                chartSize=getSize(size,new Dimension(300,200));
                DataSeries dataSeries= getChartDataSet(request, ChartType.AREA);
                LegendProperties legendProp=null;
                if (request.getParameter("refObjectName").indexOf(",")>0)
                    legendProp=legendProperties;

                //log.debug("Create AREA Chart (title - " + dataSeries.getChartTitle() + ", encoded - " + URLEncoder.encode(dataSeries.getChartTitle(),"UTF-8") + ", decoded - " + URLDecoder.decode(dataSeries.getChartTitle(),"UTF-8") + ")");
                log.debug("Create AREA Chart (title - " + dataSeries.getChartTitle() + ")");
                chart= new AxisChart( dataSeries, chartProperties, axisProperties, legendProp, chartSize.width, chartSize.height);
            }
            else if (type.equals("line"))
            {
                chartSize=getSize(size,new Dimension(300,200));
                DataSeries dataSeries= getChartDataSet(request, ChartType.LINE);
                LegendProperties legendProp=null;
                if (request.getParameter("refObjectName").indexOf(",")>0)
                    legendProp=legendProperties;

                log.debug("Create LINE Chart (title - " + dataSeries.getChartTitle() + ", encoded - " + URLEncoder.encode(dataSeries.getChartTitle(),"UTF-8") + ", decoded - " + URLDecoder.decode(dataSeries.getChartTitle(),"UTF-8") + ")");
                chart= new AxisChart( dataSeries, chartProperties, axisProperties, legendProp, chartSize.width, chartSize.height);
            }
            else if (type.equals("bar"))
            {
                chartSize=getSize(size,new Dimension(300,200));
                LegendProperties legendProp=null;
                DataSeries dataSeries;
                if (request.getParameter("refObjectName").indexOf(",")>0)
                {
                    legendProp=legendProperties;
                    dataSeries= getChartDataSet(request, ChartType.BAR_CLUSTERED);
                    log.debug("Create BAR_CLUSTERED Chart (title - " + dataSeries.getChartTitle() + ", encoded - " + URLEncoder.encode(dataSeries.getChartTitle(),"UTF-8") + ", decoded - " + URLDecoder.decode(dataSeries.getChartTitle(),"UTF-8") + ")");
                }
                else
                {
                    dataSeries= getChartDataSet(request, ChartType.BAR);
                    log.debug("Create BAR Chart (title - " + dataSeries.getChartTitle() + ", encoded - " + URLEncoder.encode(dataSeries.getChartTitle(),"UTF-8") + ", decoded - " + URLDecoder.decode(dataSeries.getChartTitle(),"UTF-8") + ")");
                }

                chart = new AxisChart( dataSeries, chartProperties, axisProperties, legendProp, chartSize.width, chartSize.height);
            }
            else if (type.equals("bar-stack"))
            {
                chartSize=getSize(size,new Dimension(300,200));
                DataSeries dataSeries= getChartDataSet(request, ChartType.BAR_STACKED);
                log.debug("Create BAR_STACKED Chart (title - " + dataSeries.getChartTitle() + ", encoded - " + URLEncoder.encode(dataSeries.getChartTitle(),"UTF-8") + ", decoded - " + URLDecoder.decode(dataSeries.getChartTitle(),"UTF-8") + ")");
                chart= new AxisChart( dataSeries, chartProperties, axisProperties, null, chartSize.width, chartSize.height);
            }

            if (chart != null && response != null) {
                try {
                    response.setContentType("image/png");
                    PNGEncoder.encode(chart, response.getOutputStream());
                } catch (IOException e) {
                    log.error("Error encoding chart as PNG", e);
                }
            } else {
                log.error("Cannot encode chart as PNG: chart or response is null");
            }
        }
        catch (UnsupportedEncodingException e) {
            log.error("DrawChart - UnsupportedEncodingException: " + e);
        }
        catch (Exception ex) {
            log.error("Cannot create chart:"+ex);
            //if (log.isDebugEnabled())
            ex.printStackTrace();
        }
    }
}
