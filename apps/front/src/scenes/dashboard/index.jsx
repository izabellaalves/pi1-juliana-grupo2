import React, { useState, useEffect } from 'react';
import { Box, Button, Typography, useTheme } from "@mui/material";
import { tokens } from "../../theme";
import Header from "../../components/Header";
import LineSpeed from "../../components/LineSpeed";
import LineAcceleration from "../../components/LineAcceleration";
import StatBox from "../../components/StatBox";
import TimerIcon from '@mui/icons-material/Timer';
import StraightenIcon from '@mui/icons-material/Straighten';
import BatterySaverIcon from '@mui/icons-material/BatterySaver';
import FlashOnIcon from '@mui/icons-material/FlashOn';
import KeyboardArrowDownIcon from '@mui/icons-material/KeyboardArrowDown';

const Dashboard = () => {
  const theme = useTheme();
  const colors = tokens(theme.palette.mode);

  // const [mediaData, setMediaData] = useState(null);
  const [realizaData, setRealizaData] = useState(null);

  const [tick, setTick] = useState(0);

  useEffect(() => {
    const intervalId = setInterval(() => {
      setTick(tick => tick + 1);
    }, 1000);

    return () => clearInterval(intervalId);
  }, []);

  const fetchData = async () => {
    try {
      /* const mediaResponse = await fetch('http://localhost:3333/media');
      if (!mediaResponse.ok) {
        throw new Error('Failed to fetch media data');
      }
      const mediaData = await mediaResponse.json();
      setMediaData(mediaData[0]);
   */
      const realizaResponse = await fetch('http://localhost:5000/data');
      if (!realizaResponse.ok) {
        throw new Error('Failed to fetch realiza data');
      }
      const realizaData = await realizaResponse.json();
      setRealizaData(realizaData);

      console.log('realizaData', realizaData);
    } catch (error) {
      console.error('Error fetching data', error);
    }
  };


  useEffect(() => {

    fetchData();
  }, [tick]);

  return (
    <Box m="15px">
      {/* HEADER */}
      <Box display="flex" justifyContent="space-between" alignItems="center">
        <Header title="CARRINHO DE LINHA" subtitle="Projeto Integrador 1 | Turma 4 | Grupo 2 " />

        {/* <Box>
          <Button
            sx={{
              backgroundColor: colors.blueAccent[700],
              color: colors.grey[100],
              fontSize: "14px",
              fontWeight: "bold",
              padding: "10px 20px",
            }}
          >
            Trajetórias Anteriores
            <KeyboardArrowDownIcon sx={{ ml: "10px" }} />
          </Button>
        </Box> */}

      </Box>

      {/* GRID & CHARTS */}
      <Box
        display="grid"
        gridTemplateColumns="repeat(12, 1fr)"
        gridAutoRows="120px"
        gap="15px"
      >
        {/* ROW 1 */}
        {realizaData && (
          <>
            {/* TEMPO */}
            <Box
              gridColumn="span 3"
              backgroundColor={colors.primary[400]}
              display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                title={`${realizaData.tempo} s`}
                subtitle="Tempo"
                progress="0.50"
                increase=""
                icon={
                  <TimerIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box>

            {/* AUTONOMIA */}
            {/* <Box
              gridColumn="span 2"
              backgroundColor={colors.primary[400]}
              display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                title={`${realizaData.consumoEnergetico}`}
                subtitle="Autonomia"
                progress="0.30"
                increase=""
                icon={
                  <BatterySaverIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box> */}

            {/* CONSUMO ENERGETICO */}
            <Box
              gridColumn="span 3"
              backgroundColor={colors.primary[400]}
              display="flex"
              alignItems="center"
              justifyContent="center"
            >
              <StatBox
                title={`${realizaData.consumoEnergetico} KWh`}
                subtitle="Consumo Energético"
                progress="0.80"
                increase=""
                icon={
                  <FlashOnIcon
                    sx={{ color: colors.greenAccent[600], fontSize: "26px" }}
                  />
                }
              />
            </Box>
          </>
        )}

        
        {/* ROW 2 */}
        {/* VELOCIDADE */}
        <Box
          gridColumn="span 6"
          gridRow="span 2"
          backgroundColor={colors.primary[400]}
        >
          <Box
            mt="25px"
            p="0 30px"
            display="flex "
            justifyContent="space-between"
            alignItems="center"
          >
            <Box>
              <Typography
                variant="h5"
                fontWeight="600"
                color={colors.grey[100]}
              >
                Velocidade Instantânea
              </Typography>
              <Typography
                variant="h4"
                fontWeight="bold"
                color={colors.greenAccent[500]}
              >
                {realizaData ? `${realizaData.velocidadeInstantanea} m/s` : 'Carregando...'}
                </Typography>
            </Box>
          </Box>
          <Box height="190px" m="-20px 0 0 0">
            <LineSpeed isDashboard={true} />
          </Box>
        </Box>
        {/* TRAJETORIA */}
        <Box
          gridColumn="span 6"
          gridRow="span 3"
          backgroundColor={colors.primary[400]}
          padding="30px"
        >
          <Typography
            variant="h5"
            fontWeight="600"
            sx={{ marginBottom: "15px" }}
          >
            Trajetória
          </Typography>
          <Box height="200px">
            {/* <GeographyChart isDashboard={true} /> */}
          </Box>
        </Box>
        {/* ACELERAÇÃO */}
        <Box
          gridColumn="span 6"
          gridRow="span 2"
          backgroundColor={colors.primary[400]}
        >
          <Box
            mt="25px"
            p="0 30px"
            display="flex "
            justifyContent="space-between"
            alignItems="center"
          >
            <Box>
              <Typography
                variant="h5"
                fontWeight="600"
                color={colors.grey[100]}
              >
                Aceleração Instantânea
              </Typography>
              <Typography
                variant="h4"
                fontWeight="bold"
                color={colors.greenAccent[500]}
              >
                {realizaData ? `${realizaData.aceleracaoInstantanea} m/s²` : 'Carregando...'}
              </Typography>
            </Box>
          </Box>
          <Box height="190px" m="-20px 0 0 0">
            <LineAcceleration isDashboard={true} />
          </Box>
        </Box> 
        

      </Box>
    </Box>
  );
};

export default Dashboard;
